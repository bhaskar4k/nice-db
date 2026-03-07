from fastapi import FastAPI
from app.routes.ingest_routes import router as ingest_router
from app.routes.health_routes import router as health_router
from metadata.db import init_db


def create_app():
    init_db()
    
    app = FastAPI(
        title="Data Ingestion Engine",
        version="1.0"
    )

    app.include_router(health_router)
    app.include_router(ingest_router)

    return app