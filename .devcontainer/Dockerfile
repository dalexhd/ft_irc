# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.187.0/containers/ubuntu/.devcontainer/base.Dockerfile

# [Choice] Ubuntu version: bionic, focal
ARG VARIANT="focal"
FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}

# [Optional] Uncomment this section to install additional OS packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    build-essential \
    python3-pip \
    valgrind \
    gdb \
    clang

RUN python3 -m pip install --upgrade pip setuptools && python3 -m pip install norminette

USER vscode
RUN git clone --depth=1 https://github.com/romkatv/powerlevel10k.git ~/powerlevel10k \
    && git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions

RUN curl -s https://static.dalexhd.dev/dev/config/.p10k.zsh > ~/.p10k.zsh
RUN curl -s https://static.dalexhd.dev/dev/config/.zshrc > ~/.zshrc
