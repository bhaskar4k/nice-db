from metadata.db import get_connection
import uuid
from app.logger import logger


def create_job(table_name, file_path):
    try:
        # Generate unique job ID
        job_id = str(uuid.uuid4())
        
        query = """
            INSERT INTO ingestion_jobs
            (job_id, table_name, file_path, status)
            VALUES (?, ?, ?, 'pending')
        """

        conn = get_connection()
        conn.execute(query, (job_id, table_name, file_path))
        conn.commit()
        conn.close()
        
        logger.info(f"Job created successfully with ID: {job_id}, table: {table_name}")
        return job_id
        
    except Exception as e:
        logger.exception(f"Error creating job for table {table_name}: {str(e)}")
        raise