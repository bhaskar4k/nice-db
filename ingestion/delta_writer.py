import os
import polars as pl
from deltalake.writer import write_deltalake, WriterProperties
from openpyxl import load_workbook
from app.logger.logger import logger

# Delta Writer
class DeltaWriter:

    def __init__(self, delta_path: str, compression="zstd"):
        self.delta_path = delta_path
        self.first_write = True

        self.writer_props = WriterProperties(
            compression=compression
        )

    def write(self, df: pl.DataFrame):

        write_deltalake(
            self.delta_path,
            df.to_arrow(),
            mode="overwrite" if self.first_write else "append",
            writer_properties=self.writer_props
        )

        self.first_write = False



# CSV / Flat File Streaming Reader
def stream_csv(filepath: str, batch_size: int):
    try:
        logger.info("stream_csv -> start")

        lf = pl.scan_csv(
            filepath,
            infer_schema_length=10000
        )

        df = lf.collect(streaming=True)

        for batch in df.iter_slices(batch_size):
            yield batch
        logger.info("stream_csv -> end")

    except Exception as e:
        logger.exception(f"stream_csv -> error: {str(e)}")
        raise
    


# Parquet Streaming Reader
def stream_parquet(filepath: str, batch_size: int):
    try:
        logger.info("stream_parquet -> start")

        lf = pl.scan_parquet(filepath)

        df = lf.collect(streaming=True)

        for batch in df.iter_slices(batch_size):
            yield batch

        logger.info("stream_parquet -> end")

    except Exception as e:
        logger.exception(f"stream_parquet -> error: {str(e)}")
        raise
    


# Excel Streaming Reader
def stream_excel(filepath: str, batch_size: int):
    try:
        logger.info("stream_excel -> start")

        wb = load_workbook(filepath, read_only=True)
        ws = wb.active

        rows = ws.iter_rows(values_only=True)

        header = next(rows)

        batch = []

        for row in rows:

            batch.append(row)

            if len(batch) >= batch_size:

                yield pl.DataFrame(batch, schema=header)
                batch.clear()

        if batch:
            yield pl.DataFrame(batch, schema=header)

        wb.close()

        logger.info("stream_excel -> end")

    except Exception as e:
        logger.exception(f"stream_excel -> error: {str(e)}")
        raise

    

# Reader Factory
def get_reader(filepath: str, batch_size: int):
    try:
        logger.info("get_reader -> start")

        ext = os.path.splitext(filepath)[1].lower()

        if ext in [".csv", ".tsv", ".txt"]:
            logger.info("get_reader -> using stream_csv")
            return stream_csv(filepath, batch_size)

        if ext == ".parquet":
            logger.info("get_reader -> using stream_parquet")
            return stream_parquet(filepath, batch_size)

        if ext in [".xlsx", ".xls"]:
            logger.info("get_reader -> using stream_excel")
            return stream_excel(filepath, batch_size)

        raise ValueError(f"Unsupported file type {ext}")
    
    except Exception as e:
        logger.exception(f"get_reader -> error: {str(e)}")
        raise



# Main Ingestion Function
def stream_file_to_deltalake(filepath: str):
    try:
        logger.info("stream_file_to_deltalake -> start")

        delta_path = "E:\\Project\\NICE-DB-DELTALAKE"
        batch_size = 500_000

        reader = get_reader(filepath, batch_size)
        writer = DeltaWriter(delta_path)

        for batch_df in reader:
            writer.write(batch_df)

        logger.info("stream_file_to_deltalake -> end")

    except Exception as e:
        logger.exception(f"stream_file_to_deltalake -> error: {str(e)}")
        raise