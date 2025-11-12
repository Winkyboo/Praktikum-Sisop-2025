awk -F, 'NR>1 && $2 ~/2/ && $2 ! ~/_/ {print $2}' DataStreamer.csv | sort
awk -F, 'NR>1 && $2 ~/2/ && $2 ! ~/_/ {count++} END {print "\nJumlah username yang sesuai:", count}' DataStreamer.csv
