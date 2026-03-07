from fastapi import APIRouter
from app.services.ingest_service import create_ingestion_job
from app.models.job_model import IngestionRequest
from app.logger import logger

router = APIRouter(prefix="/ingest")


@router.post("/ingest_data")
def ingest_data(request: IngestionRequest):
    try:
        logger.info(f"ingest_data -> start")

        job_id = create_ingestion_job(request)

        logger.info(f"ingest_data -> end")
        return {"job_id": job_id}
    
    except Exception as e:
        logger.exception(f"ingest_data -> error: {str(e)}")
        raise