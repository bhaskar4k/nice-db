import json
from pathlib import Path

def load_config():
    """Load configuration from config.json"""
    BASE_DIR = Path(__file__).resolve().parent.parent.parent
    CONFIG_FILE = BASE_DIR / "config.json"
    
    try:
        with open(CONFIG_FILE, 'r') as f:
            return json.load(f)
    except FileNotFoundError:
        raise FileNotFoundError(f"Configuration file not found at {CONFIG_FILE}")
    except Exception as e:
        raise Exception(f"Failed to load configuration from {CONFIG_FILE}: {str(e)}")
