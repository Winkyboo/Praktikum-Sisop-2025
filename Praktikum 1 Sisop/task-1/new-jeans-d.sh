tr -d '\r' < DataStreamer.csv | awk -F',' '
NR>1 {DeviceCount[$7]++; DeviceStream[$7] += $4}
END {
    maxDevice = "";
    maxCount = 0;
    maxStream = "";
    streamCount = 0;
    maxloyal = 0;
    loyal = 0;
    loyaldevice = "";

    for (Device in DeviceCount) {
        loyal = DeviceStream[Device] / DeviceCount[Device];
        if (maxloyal < loyal) {
            maxloyal = loyal;
            loyaldevice = Device;
        }
        if (DeviceCount[Device] > maxCount) {
            maxCount = DeviceCount[Device];
            maxDevice = Device;
        }
        if (DeviceStream[Device] > streamCount) {
            maxStream = Device;
            streamCount = DeviceStream[Device];
        }
        print "Device Name:", Device, "|Users:", DeviceCount[Device], "|Stream Time:", DeviceStream[Device];
    }

    print "Most used device by user count is", maxDevice, "with", maxCount, "users.";
    print "Most used device by stream time is", maxStream, "with", streamCount, "[Time Unit]";
    print "And the most loyal device is", loyaldevice, "with an average stream time of", maxloyal, "[Time Unit] per device";
}'
