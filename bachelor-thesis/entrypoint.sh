#!/bin/bash

if [[ "$1" == "clean" ]]; then
    rm -f bachelor-thesis.{bib,aux,log,bbl,bcf,blg,run.xml,toc,tct,pdf,out}
else
    xelatex bachelor-thesis
    biber   bachelor-thesis
    xelatex bachelor-thesis
    xelatex bachelor-thesis

    rm -f bachelor-thesis.{bib,aux,log,bbl,bcf,blg,run.xml,toc,tct,out}
fi
