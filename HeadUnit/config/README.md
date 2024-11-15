# Configuration
## Path of Configuration files
- Configuration files (commonapi.ini and vsomeip.json) need to be in a same path with execution files.
- In our case, the config files are included in /etc/commonapi folder and the following commands are used to set the path.
    ```
    export VSOMEIP_CONFIGURATION_FILE=/etc/commonapi
    export COMMONAPI_CONFIG=/etc/commonapi
    ```
## commonapi.ini
- Define Connection between CommonAPI Interface and Binding Library for Runtime level
- Binding Library can be changed by modifying this file

## vsomeip.json
- Service ID and Instance ID is defined in .fdepl file
- In vsomeip.json, we can set many variables about vsomeip communication
  unicast address
- vsomeip logging option (include dlt)
- service port (reliable-tcp / unreliable-udp)
- routing manager
- multicast address, port and protocol(tcp/udp) for service discovery

## Reference 
- [vsomeip in 10 minutes](https://github.com/COVESA/vsomeip/wiki/vsomeip-in-10-minutes)
- [CommonAPI C SomeIP in 10 minutes](https://github.com/COVESA/capicxx-someip-tools/wiki/CommonAPI-C---SomeIP-in-10-minutes)
