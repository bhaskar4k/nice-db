from metadata.job_repository import create_job
from app.models.job_model import IngestionRequest

def create_ingestion_job(request: IngestionRequest):

    job_id = create_job(
        table_name=request.table,
        file_path=request.path
    )

    return job_id