#!/bin/bash
echo "Script agg_5min_to_hour.sh dijalankan pada $(date)" >> /home/$USER/sisop/praktikum1/cron.log
LOG_DIR=$"/home/$USER/metrics"

CURRENT_HOUR=$(date +"%Y%m%d%H")

AGG_LOG_FILE="$LOG_DIR/metrics_agg_$CURRENT_HOUR*.log"

cat $LOG_DIR/metrics_$CURRENT_HOUR*.log | awk -F, 'NR==1 {print} NR>1 {print}' > $AGG_LOG_FILE

chmod 600 $AGG_LOG_FILE
