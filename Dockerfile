FROM evbauer/mesa_lean:9793.01

USER root

RUN apt update --fix-missing && apt install -y --no-install-recommends \
        build-essential \  
        gdb \
        g++ \
        valgrind \
	vim \
        tree \
        x11-apps \
        libsdl2-dev 

# Set the working directory in the container
WORKDIR /app

# Expose bash(default shell to connect later)
CMD ["bash"]
