from repository.job_repository import create, get_all, get_first_pending_job
from app.models.job_model import IngestionRequest
from app.logger.logger import logger


# Create a new ingestion job
def create_ingestion_job(payload: IngestionRequest):
    try:
        logger.info(f"Creating ingestion job for table: {payload.table}, path: {payload.path}")
        
        job_id = create(payload)
        
        logger.info(f"Ingestion job created successfully with ID: {job_id}")
        return True
        
    except Exception as e:
        logger.exception(f"Error creating ingestion job: {str(e)}")
        raise



# Get all ingestion jobs
def get_all_ingestion_jobs():
    try:
        logger.info("get_all_ingestion_jobs -> start")
        
        jobs = get_all()
        
        logger.info(f"get_all_ingestion_jobs -> end")
        return jobs
    
    except Exception as e:
        logger.exception(f"get_all_ingestion_jobs -> error: {str(e)}")
        raise



# Get the first pending ingestion job
def get_the_first_pending_ingestion_job():
    try:
        logger.info("get_the_first_pending_ingestion_job -> start")
        
        job = get_first_pending_job()
        
        logger.info(f"get_the_first_pending_ingestion_job -> end")
        return job
    
    except Exception as e:
        logger.exception(f"get_the_first_pending_ingestion_job -> error: {str(e)}")
        raise