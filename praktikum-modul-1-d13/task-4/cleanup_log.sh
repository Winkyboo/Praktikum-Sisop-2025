#!/bin/bash
echo "Script cleanup_log.sh dijalankan pada $(date)" >> /home/$USER/sisop/praktikum1/cron.log

LOG_DIR="/home/$USER/metrics"

CURRENT_HOUR=$(date +"%Y%m%d%H")

find $LOG_DIR -name "metrics_agg_*.log" -mtime +0.5 -exec rm -f {} \;
