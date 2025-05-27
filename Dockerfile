# Base image with GCC and build tools
#FROM ubuntu:22.04
FROM evbauer/mesa_lean:r24.03.1.01

USER root

# Install required tools
RUN apt update && apt upgrade -y

RUN apt update --fix-missing && apt install -y --no-install-recommends \
        build-essential \  
        gdb \
        valgrind \
        clang \
        x11-apps \
        cmake \
        nano \
        git \
        libsdl2-dev 

# Set the working directory in the container
WORKDIR /app

# Expose bash(default shell to connect later)
CMD ["bash"]