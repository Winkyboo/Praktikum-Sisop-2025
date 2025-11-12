awk -F, 'NR>1 && $2 ~ /[0-9]/ {count[$3]++}
> END{
> countmax=0;
> songmax="";
> for(song in count){
> if(count[song]>countmax){
> countmax=count[song];
> songmax=song;
> }
> }
> print "Lagu yang paling banyak di-streaming oleh user yang memiliki username berangka: ", songmax;
> print "Jumlah usernya: ", countmax;
> 
> if(countmax>24){
> print "Keren, Minji! Kamu hebat <3";
> }
> else{
> print "Maaf, Minji, total streamingnya tidak sesuai harapan :(";
> }
> }
> ' DataStreamer.csv
