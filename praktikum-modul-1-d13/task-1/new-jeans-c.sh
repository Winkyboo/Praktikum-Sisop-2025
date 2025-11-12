awk -F, '$2 == "Cookie" {print "Lagu : ", $2, "\nAlbum: ", $1, "\nTahun rilis: ", $3}' AlbumDetails.csv
