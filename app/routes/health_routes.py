from fastapi import APIRouter
from app.logger import logger

router = APIRouter(prefix="/health")


@router.get("/health_check")
def health_check():
    try:
        logger.debug("Health check endpoint called")
        return {
            "status": "healthy"
        }
    except Exception as e:
        logger.exception(f"Error in health_check endpoint: {str(e)}")
        raise