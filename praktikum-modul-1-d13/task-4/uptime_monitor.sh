#!/bin/bash
echo "Script uptime_monitor.sh dijalankan pada $(date)" >> /home/$USER/sisop/praktikum1/cron.log

LOG_DIR="/home/$USER/metrics"

CURRENT_HOUR=$(date +"%Y%m%d%H")

UPTIME_LOG_FILE="$LOG_DIR/uptime_$CURRENT_HOUR.log"

UPTIME=$(uptime)
LOAD_AVG=$(cat /proc/loadavg | awk '{print $1, $2, $3}')

echo "uptime,load_avg_1min,load_avg_5min,load_avg_15min" > $UPTIME_LOG_FILE
echo "$UPTIME,$LOAD_AVG" >> $UPTIME_LOG_FILE

chmod 600 $UPTIME_LOG_FILE
