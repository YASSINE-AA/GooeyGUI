# Setting a base image to install 

FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive



RUN apt-get update && apt-get install -y \
    build-essential \ 
    vim \ 
    make \ 
    cmake \ 
    git \ 
    curl \ 
    libssl-dev \ 
    libcurl4-openssl-dev \ 
    pkg-config \ 
    x11-apps \ 
    libx11-6 libx11-dev \ 
    libxext-dev libxrandr-dev libxinerama-dev \ 
    libxcursor-dev libxi-dev \        
    && rm -rf /var/lib/apt/lists/*



# Creaing a user 
RUN useradd -m X11User

USER X11User

# set the working directory 
WORKDIR /Gooey
# clone
RUN git clone https://github.com/YASSINE-AA/GooeyGUI.git --recursive


# Only enabling the X11 Build 
# RUN cmake -S . -B build -DGLFW_BUILD_X11=ON -DGLFW_BUILD_WIN32=OFF -DGLFW_BUILD_COCOA=OFF -DGLFW_BUILD_WAYLAND=OFF

# WORKDIR /starter/firmware


# RUN bash 

CMD ["bash"]