while True:

    job = fetch_pending_job()

    if job:
        process_job(job)