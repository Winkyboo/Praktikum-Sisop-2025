#!/bin/bash
echo "Script minute5_log.sh dijalankan pada $(date)" >> /home/$USER/sisop/praktikum1/cron.log

LOG_DIR="/home/$USER/metrics"
mkdir -p "$LOG_DIR"

CURRENT_TIME=$(date +"%Y%m%d%H%M%S")

LOG_FILE="$LOG_DIR/metrics_$CURRENT_TIME.log"

RAM_INFO=$(free -m | grep Mem)
mem_total=$(echo $RAM_INFO | awk '{print $2}')
mem_used=$(echo $RAM_INFO | awk '{print $3}')
mem_free=$(echo $RAM_INFO | awk '{print $4}')
mem_shared=$(echo $RAM_INFO | awk '{print $5}')
mem_buff=$(echo $RAM_INFO | awk '{print $6}')
mem_available=$(echo $RAM_INFO | awk '{print $7}')

SWAP_INFO=$(free -m | grep Swap)
swap_total=$(echo $SWAP_INFO |awk '{print $2}')
swap_used=$(echo $SWAP_INFO |awk '{print $3}')
swap_free=$(echo $SWAP_INFO |awk '{print $4}')

DIR_SIZE=$(du -sh /home/$USER | awk '{print $1'})

echo "mem_total,mem_used,mem_free,mem_shared,mem_buff,mem_available,swap_total,swap_used,swap_free,path,path_size" > $LOG_FILE 
echo "$mem_total,$mem_used,$mem_free,$mem_shared,$mem_buff,$mem_available,$swap_total,$swap_used,$swap_free,/home/$USER/,$DIR_SIZE" >> $LOG_FILE

chmod 600 $LOG_FILE  
