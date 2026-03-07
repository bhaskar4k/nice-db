import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

import threading
from threading import Lock, Semaphore
from apscheduler.schedulers.background import BackgroundScheduler
from apscheduler.triggers.interval import IntervalTrigger
from app.logger import logger
from ingestion.worker_logic import process_job, fetch_pending_job
import atexit
from app.utils.config import load_config

CONFIG = load_config()

MAX_CONCURRENT_JOB = CONFIG["threading"]["max_parallel_workers"]
JOB_FETCH_INTERVAL = CONFIG["threading"]["job_fetch_interval_seconds"]


job_lock = Lock()
max_concurrent_jobs = Semaphore(MAX_CONCURRENT_JOB)
scheduler_lock = Lock()


active_jobs = set()


def safe_process_job(job_id):
    """Process a job with thread-safe locking and semaphore"""
    try:
        # Acquire semaphore - blocks if max concurrent jobs reached
        with max_concurrent_jobs:
            # Acquire lock for critical section
            with job_lock:
                if job_id in active_jobs:
                    logger.warning(f"Job {job_id} already being processed, skipping")
                    return
                
                active_jobs.add(job_id)
                logger.info(f"Starting job processing: {job_id}")
            
            try:
                # Process job outside the lock to avoid holding it too long
                process_job(job_id)
                
                # Update completion status under lock
                with job_lock:
                    active_jobs.discard(job_id)
                    logger.info(f"Job processing completed: {job_id}")
            
            except Exception as e:
                with job_lock:
                    active_jobs.discard(job_id)
                logger.exception(f"Error processing job {job_id}: {str(e)}")
    
    except Exception as e:
        logger.exception(f"Failed to acquire semaphore for job {job_id}: {str(e)}")


def cron_job():
    """Scheduled job that fetches and processes pending jobs"""
    try:
        with scheduler_lock:
            logger.debug("Cron job triggered - fetching pending jobs")
            
            job = fetch_pending_job()
            
            if job:
                job_id = job.get("id") if isinstance(job, dict) else job.id
                
                # Process job in a separate thread to avoid blocking scheduler
                worker_thread = threading.Thread(
                    target=safe_process_job,
                    args=(job_id,),
                    daemon=False,
                    name=f"worker-thread-{job_id}"
                )
                worker_thread.start()
            else:
                logger.debug("No pending jobs found")
    
    except Exception as e:
        logger.exception(f"Error in cron job: {str(e)}")


def start_scheduler():
    """Start the background scheduler with cron job"""
    try:
        scheduler = BackgroundScheduler()
        
        # Add job with interval trigger (runs every JOB_FETCH_INTERVAL seconds)
        scheduler.add_job(
            cron_job,
            trigger=IntervalTrigger(seconds=JOB_FETCH_INTERVAL),
            id='ingest_cron_job',
            name='Ingestion Cron Job',
            replace_existing=True,
            max_instances=1  # Only one instance of this job can run at a time
        )
        
        scheduler.start()
        print(f"Scheduler started - job runs every {JOB_FETCH_INTERVAL} seconds")
        logger.info(f"Scheduler started - job runs every {JOB_FETCH_INTERVAL} seconds")
        
        # Gracefully shutdown scheduler on exit
        atexit.register(lambda: scheduler.shutdown())
        
        return scheduler
    
    except Exception as e:
        logger.exception(f"Failed to start scheduler: {str(e)}")
        raise