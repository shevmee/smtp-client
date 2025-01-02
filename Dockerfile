# Build Stage
FROM ubuntu:22.04 as builder

# Define arguments for versioning
ARG CMAKE_VERSION=3.28.0
ARG BOOST_VERSION=1.83.0
ARG BOOST_VERSION_UNDERSCORE=${BOOST_VERSION//./_}

# Set non-interactive mode
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    libssl-dev \
    wget \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Install specific CMake version
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.tar.gz && \
    tar -xvf cmake-${CMAKE_VERSION}-linux-x86_64.tar.gz --strip-components=1 -C /usr/local && \
    rm cmake-${CMAKE_VERSION}-linux-x86_64.tar.gz

# Install Boost
RUN wget https://boostorg.jfrog.io/artifactory/main/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION_UNDERSCORE}.tar.gz && \
    tar -xvf boost_${BOOST_VERSION_UNDERSCORE}.tar.gz && \
    cd boost_${BOOST_VERSION_UNDERSCORE} && \
    ./bootstrap.sh --prefix=/usr/local && \
    ./b2 install && \
    cd .. && \
    rm -rf boost_${BOOST_VERSION_UNDERSCORE} boost_${BOOST_VERSION_UNDERSCORE}.tar.gz

# Set library path
ENV LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"

# Build application
WORKDIR /app
COPY . .
RUN rm -rf build && cmake -Bbuild -H. && cmake --build build

# Runtime Stage
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    libssl-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Set library path
ENV LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"

# Copy application from build stage
WORKDIR /app
COPY --from=builder /app/build ./build
COPY --from=builder /usr/local/lib /usr/local/lib

# Set default user for security
RUN useradd -ms /bin/bash appuser
USER appuser

# Define default command
CMD ["./build/client"]
