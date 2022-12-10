# foxnit

Foxnit is a runit wrapper, something similar to systemctl or rc-update, etc. 

# Installing
```bash
make # to build
sudo make install # to install
```
## getting started
`foxnit -h`
```
Usage: foxnit [COMMAND] [OPTION] SERVICE_NAME
Options:
  -h, --help                    Print this help message.
  -v, --version                 Print version.

Usage:
  foxnit services               List all services.
  foxnit services register      Register and run a service.
  foxnit services unregister    Unregister and stop service.
  foxnit services start         Start a service.
  foxnit services stop          Stop a service.
  foxnit services restart       Restart a service.
  foxnit services status        Get status of a service.
```