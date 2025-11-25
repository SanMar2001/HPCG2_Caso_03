#!/bin/bash
WORKERS=("wn1" "wn2" "wn3" "head")
REPO_PATH="/home/cladmin/HPCG2_Caso_03"

for W in "${WORKERS[@]}"; do
    ssh $W "cd $REPO_PATH && git pull"
done
