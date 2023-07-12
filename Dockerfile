FROM ubuntu:latest

# Install dependencies
RUN apt-get update && \
    apt-get install -y build-essential && \
    apt-get install -y gfortran && \
    apt-get install -y libopenblas-dev 
    
RUN apt install -y cmake

# Set environment variables
ENV LD_LIBRARY_PATH=/usr/local/lib

# Copy and compile your code
WORKDIR /simulator

COPY . .

WORKDIR ./build

RUN cmake ..

RUN make

CMD ["./simulator", "../examples/bell.qasm"]
