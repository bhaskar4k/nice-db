from repository.db import get_connection
import uuid
from app.models.job_model import IngestionRequest
from app.logger import logger
from app.enum.master_enum import JobStatus

# Create a new ingestion job
def create(payload: IngestionRequest):
    try:
        logger.info("create -> start")

        # Generate unique job ID
        job_id = str(uuid.uuid4())
        
        query = """
            INSERT INTO ingestion_jobs
            (job_id, table_name, file_path)
            VALUES (?, ?, ?)
        """

        conn = get_connection()
        conn.execute(query, (job_id, payload.table, payload.path))
        conn.commit()
        conn.close()
        
        logger.info(f"create -> end")
        return job_id
        
    except Exception as e:
        logger.exception(f"create -> error: {str(e)}")
        raise



# Get all ingestion jobs
def get_all():
    try:
        logger.info("get_all -> start")
        
        query = "SELECT * FROM ingestion_jobs"
        
        conn = get_connection()
        cursor = conn.execute(query)
        jobs = cursor.fetchall()
        column_names = [desc[0] for desc in cursor.description]
        conn.close()
        
        # Convert tuples to dictionaries
        job_objects = [dict(zip(column_names, job)) for job in jobs]
        
        logger.info(f"get_all -> end")
        return job_objects
    
    except Exception as e:
        logger.exception(f"get_all -> error: {str(e)}")
        raise



# Get the first pending ingestion job
def get_first_pending_job():
    try:
        logger.info("get_first_pending_job -> start")
        
        query = "SELECT * FROM ingestion_jobs WHERE status = ? LIMIT 1"
        
        conn = get_connection()
        cursor = conn.execute(query, (JobStatus.PENDING.value,))
        job = cursor.fetchone()
        column_names = [desc[0] for desc in cursor.description]
        conn.close()

        if job:
            job_object = dict(zip(column_names, job))
            logger.info("get_first_pending_job -> end")
            return job_object

        logger.info("get_first_pending_job -> end")
        return None

    except Exception as e:
        logger.exception(f"get_first_pending_job -> error: {str(e)}")
        raise