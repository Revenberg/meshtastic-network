FROM python:3.11-slim
WORKDIR /app
COPY reader.py .
RUN pip install paho-mqtt
CMD ["python", "reader.py"]
