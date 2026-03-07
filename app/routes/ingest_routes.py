from fastapi import APIRouter
from app.services.ingest_service import create_ingestion_job
from app.models.job_model import IngestionRequest
from app.logger import logger

router = APIRouter(prefix="/ingest")


@router.post("/ingest_data")
def ingest_data(request: IngestionRequest):
    try:
        logger.info(f"Received ingest request for table: {request.table}")
        job_id = create_ingestion_job(request)
        logger.info(f"Ingest endpoint returning job_id: {job_id}")
        return {"job_id": job_id}
    except Exception as e:
        logger.exception(f"Error in ingest_data endpoint: {str(e)}")
        raise