from repository.job_repository import create, get_all
from app.models.job_model import IngestionRequest
from app.logger import logger

def create_ingestion_job(payload: IngestionRequest):
    try:
        logger.info(f"Creating ingestion job for table: {payload.table}, path: {payload.path}")
        
        job_id = create(payload)
        
        logger.info(f"Ingestion job created successfully with ID: {job_id}")
        return job_id
        
    except Exception as e:
        logger.exception(f"Error creating ingestion job: {str(e)}")
        raise


def get_all_jobs():
    try:
        logger.info("get_all_jobs -> start")
        
        jobs = get_all()
        
        logger.info(f"get_all_jobs -> end")
        return jobs
    
    except Exception as e:
        logger.exception(f"get_all_jobs -> error: {str(e)}")
        raise