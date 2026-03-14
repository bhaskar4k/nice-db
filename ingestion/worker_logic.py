import polars as pl
from repository.job_repository import update_job_status, update_job_rows_processed
from app.enum.master_enum import JobStatus
from app.logger.logger import logger
from .delta_writer import stream_file_to_deltalake


def process_job(job):
    try:
        logger.info("process_job -> start")
        # Extract job_id
        job_id = job.get("job_id") if isinstance(job, dict) else job.job_id

        # Update job status to IN_PROGRESS
        update_job_status(job_id, JobStatus.IN_PROGRESS.value)

        file_path = job.get("file_path") if isinstance(job, dict) else job.file_path

        stream_file_to_deltalake(file_path)

        # Update job status to COMPLETED
        update_job_status(job_id, JobStatus.COMPLETED.value)
        logger.info("process_job -> end")

    except Exception as e:
        # Update job status to FAILED
        job_id = job.get("job_id") if isinstance(job, dict) else job.job_id
        update_job_status(job_id, JobStatus.FAILED.value)

        logger.exception(f"process_job -> error: {str(e)}")
        raise