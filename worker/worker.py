from ingestion.worker_logic import process_job, fetch_pending_job

while True:

    job = fetch_pending_job()

    if job:
        process_job(job)