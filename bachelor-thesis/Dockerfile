FROM ubuntu:mantic

WORKDIR /work
ARG DEBIAN_FRONTEND=noninteractive

# Install TinyTex. Perl is needeed by TinyTex, libfontconfig1 is needed by xelatex.
RUN apt update && apt install -y perl wget libfontconfig1 && \
  wget -qO- "https://yihui.org/tinytex/install-bin-unix.sh" | sh

ENV PATH="${PATH}:/root/bin"

# Install Microsoft fonts.
RUN echo ttf-mscorefonts-installer msttcorefonts/accepted-mscorefonts-eula select true | debconf-set-selections
RUN apt update && apt install -y ttf-mscorefonts-installer fontconfig && fc-cache -f -v

# Install LaTeX packages neede by the thesis template.
RUN tlmgr install \
  biber extsizes babel-english babel-russian csquotes \
  oberdiek colortbl fancyhdr caption floatrow \
  hyphen-russian algorithms algorithmicx enumitem setspace\
  biblatex biblatex-gost totcount listings chngcntr \
  titlesec ulem was pgf filecontents && \
  tlmgr path add
