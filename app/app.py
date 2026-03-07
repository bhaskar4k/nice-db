import json
from pathlib import Path
from fastapi import FastAPI
from app.routes.ingest_routes import router as ingest_router
from app.routes.health_routes import router as health_router
from repository.db import init_db
from app.logger import logger

# Load configuration
BASE_DIR = Path(__file__).resolve().parent.parent
CONFIG_FILE = BASE_DIR / "config.json"

with open(CONFIG_FILE, 'r') as f:
    CONFIG = json.load(f)


def create_app():
    try:
        logger.info("Creating application")

        init_db()
        
        app = FastAPI(
            title=CONFIG["app"]["name"],
            version=CONFIG["app"]["version"],
            description="FastAPI-based data ingestion platform with Delta Lake integration"
        )

        app.include_router(health_router)
        app.include_router(ingest_router)
        
        logger.info(f"Application '{CONFIG['app']['name']}' v{CONFIG['app']['version']} started successfully")

        return app
        
    except Exception as e:
        logger.exception("Failed to create application")
        raise