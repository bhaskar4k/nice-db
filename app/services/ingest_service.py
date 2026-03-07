from metadata.job_repository import create_job
from app.models.job_model import IngestionRequest
from app.logger import logger

def create_ingestion_job(request: IngestionRequest):
    try:
        logger.info(f"Creating ingestion job for table: {request.table}, path: {request.path}")
        
        job_id = create_job(
            table_name=request.table,
            file_path=request.path
        )
        
        logger.info(f"Ingestion job created successfully with ID: {job_id}")
        return job_id
        
    except Exception as e:
        logger.exception(f"Error creating ingestion job: {str(e)}")
        raise