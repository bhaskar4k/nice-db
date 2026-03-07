from metadata.db import get_connection

def create_job(table_name, file_path):

    # job_id = generate_uuid()
    job_id = "abcd"

    query = """
        INSERT INTO ingestion_jobs
        (job_id, table_name, file_path, status)
        VALUES (?, ?, ?, 'pending')
    """

    conn = get_connection()
    conn.execute(query, (job_id, table_name, file_path))
    conn.commit()

    return job_id