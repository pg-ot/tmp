# Deployment Scripts

Quick deployment scripts for the IEC 61850 GOOSE CTF environment.

## Prerequisites

1. Docker and Docker Compose installed
2. OpenPLC and ScadaBR images loaded:
   ```bash
   docker load -i openplc-configured.tar
   docker load -i scadabr-configured.tar
   ```

## Quick Start

### Deploy Everything
```bash
./deploy.sh
```

### Check Status
```bash
./status.sh
```

### Restart
```bash
./restart.sh
```

### Stop
```bash
./stop.sh
```

## Manual Deployment

```bash
# Start
docker-compose -f docker-compose-ctf-final.yml up -d

# Stop
docker-compose -f docker-compose-ctf-final.yml down

# View logs
docker-compose -f docker-compose-ctf-final.yml logs -f

# Restart specific service
docker-compose -f docker-compose-ctf-final.yml restart openplc
```

## Architecture

### Networks
- **mahashakti** (192.168.100.0/24): IEDs and GOOSE traffic
- **aushadi_raksha** (192.168.200.0/24): OpenPLC and ScadaBR

### Containers
- **substation-breaker-v1**: Vulnerable breaker (port 9001)
- **substation-breaker-v2**: Secure breaker (port 9002)
- **substation-control-ied**: GOOSE publisher
- **kali-workstation**: Attacker workstation (dual network)
- **openplc**: PLC runtime (port 8081, Modbus 502)
- **scadabr**: SCADA HMI (port 8080)

## Access

### From Host
- Breaker v1: http://localhost:9001
- Breaker v2: http://localhost:9002
- OpenPLC: http://localhost:8081 (openplc/openplc)
- ScadaBR: http://localhost:8080/ScadaBR (admin/admin)
- Modbus: localhost:502

### From Kali
```bash
docker exec -it kali-workstation bash

# Aushadi Raksha network (SCADA)
curl http://192.168.200.3:8080
curl http://192.168.200.4:8080

# Mahashakti network (IEDs)
curl http://192.168.100.4:9000
curl http://192.168.100.2:9000
```

## Troubleshooting

### Containers not starting
```bash
# Check logs
docker-compose -f docker-compose-ctf-final.yml logs

# Rebuild
docker-compose -f docker-compose-ctf-final.yml build --no-cache
docker-compose -f docker-compose-ctf-final.yml up -d
```

### Port conflicts
Edit `docker-compose-ctf-final.yml` and change host ports:
```yaml
ports:
  - "9080:8080"  # Change 9080 to any free port
```

### Network issues
```bash
# Recreate networks
docker-compose -f docker-compose-ctf-final.yml down
docker network prune
docker-compose -f docker-compose-ctf-final.yml up -d
```

## Advanced Management

For comprehensive management, use the event management tools:
```bash
cd ../event-management
./ctf-admin.sh
```
