# Complete Deployment Guide

## Overview

This guide covers deploying the IEC 61850 GOOSE CTF environment with integrated SCADA systems (OpenPLC + ScadaBR).

## What's Included

### Updated Files
1. **docker-compose-ctf-final.yml** - Main deployment configuration
   - ✅ OpenPLC service added
   - ✅ ScadaBR service added
   - ✅ Dual network architecture
   - ✅ Kali connected to both networks

2. **Deployment Scripts** (in `deployment/`)
   - `deploy.sh` - One-command deployment
   - `stop.sh` - Stop all containers
   - `restart.sh` - Restart containers
   - `status.sh` - Check deployment status

3. **Event Management Scripts** (in `event-management/`)
   - `ctf-admin.sh` - Comprehensive admin tool (UPDATED)
   - `status.sh` - Status checker (UPDATED)
   - `monitor.sh` - Live monitoring (UPDATED)
   - All other management scripts

## Quick Deployment

### Option 1: Using Deployment Script (Recommended)
```bash
cd deployment
chmod +x *.sh
./deploy.sh
```

### Option 2: Using Docker Compose
```bash
cd deployment
docker-compose -f docker-compose-ctf-final.yml up -d
```

### Option 3: Using Admin Tool
```bash
cd event-management
chmod +x *.sh
./ctf-admin.sh
# Select: Deployment > Deploy CTF Environment
```

## Verification

### Check Status
```bash
cd deployment
./status.sh
```

Expected output:
```
=== CTF Environment Status ===

NAME                     STATUS
scadabr                  Up
openplc                  Up
kali-workstation         Up
substation-breaker-v1    Up
substation-breaker-v2    Up
substation-control-ied   Up

=== Network Configuration ===

Kali Workstation Networks:
  Power Plant:   192.168.100.5
  SCADA Network: 192.168.200.3
```

### Test Connectivity
```bash
# From Kali to SCADA
docker exec kali-workstation bash -c "timeout 5 bash -c 'cat < /dev/null > /dev/tcp/openplc/8080' && echo 'OpenPLC OK'"

# From Kali to IEDs
docker exec kali-workstation bash -c "timeout 5 bash -c 'cat < /dev/null > /dev/tcp/substation-breaker-v1/9000' && echo 'Breaker OK'"
```

## Access URLs

### From Host Machine
| Service | URL | Credentials |
|---------|-----|-------------|
| Breaker v1 | http://localhost:9001 | - |
| Breaker v2 | http://localhost:9002 | - |
| OpenPLC | http://localhost:8081 | openplc/openplc |
| ScadaBR | http://localhost:8080/ScadaBR | admin/admin |
| Modbus TCP | localhost:502 | - |

### From Kali Container
```bash
docker exec -it kali-workstation bash

# Access SCADA systems
curl http://openplc:8080
curl http://scadabr:8080

# Access IEDs
curl http://substation-breaker-v1:9000
curl http://substation-breaker-v2:9000

# Modbus communication
python3 -c "from pymodbus.client import ModbusTcpClient; c = ModbusTcpClient('openplc', port=502)"
```

## Network Architecture

```
┌─────────────────────────────────────────────────────────┐
│  Power Plant Network (192.168.100.0/24)                 │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐              │
│  │Breaker v1│  │Breaker v2│  │Control   │              │
│  │(9001)    │  │(9002)    │  │IED       │              │
│  └──────────┘  └──────────┘  └──────────┘              │
│         │            │              │                   │
│         └────────────┴──────────────┘                   │
│                      │                                  │
│               ┌──────┴──────┐                           │
│               │    Kali     │                           │
│               │ Workstation │                           │
│               └──────┬──────┘                           │
└──────────────────────┼──────────────────────────────────┘
                       │
┌──────────────────────┼──────────────────────────────────┐
│  SCADA Network (192.168.200.0/24)                       │
│               ┌──────┴──────┐                           │
│         ┌─────┴─────┐ ┌─────┴─────┐                    │
│         │  OpenPLC  │ │  ScadaBR  │                    │
│         │  (8081)   │ │  (8080)   │                    │
│         │  Modbus   │ │           │                    │
│         │  (502)    │ │           │                    │
│         └───────────┘ └───────────┘                    │
└─────────────────────────────────────────────────────────┘
```

## Management Commands

### Start/Stop
```bash
cd deployment

# Start
./deploy.sh

# Stop
./stop.sh

# Restart
./restart.sh

# Status
./status.sh
```

### Using Docker Compose
```bash
cd deployment

# Start
docker-compose -f docker-compose-ctf-final.yml up -d

# Stop
docker-compose -f docker-compose-ctf-final.yml down

# Restart
docker-compose -f docker-compose-ctf-final.yml restart

# Logs
docker-compose -f docker-compose-ctf-final.yml logs -f

# Specific service
docker-compose -f docker-compose-ctf-final.yml restart openplc
```

### Using Admin Tool
```bash
cd event-management
./ctf-admin.sh
```

Features:
- Installation & Setup
- Deployment management
- Updates & Git operations
- Live monitoring
- System information
- Team access details

## Troubleshooting

### Containers Not Starting
```bash
# Check logs
docker-compose -f docker-compose-ctf-final.yml logs

# Check specific container
docker logs openplc
docker logs scadabr
```

### Port Conflicts
Edit `docker-compose-ctf-final.yml`:
```yaml
ports:
  - "9080:8080"  # Change host port
```

### Network Issues
```bash
# Recreate networks
docker-compose -f docker-compose-ctf-final.yml down
docker network prune -f
docker-compose -f docker-compose-ctf-final.yml up -d
```

### Kali Not on Both Networks
```bash
# Check networks
docker inspect kali-workstation --format="{{json .NetworkSettings.Networks}}"

# Reconnect
docker network connect deployment_aushadi_raksha kali-workstation
```

### SCADA Images Missing
```bash
# Load images
docker load -i openplc-configured.tar
docker load -i scadabr-configured.tar

# Verify
docker images | grep -E "openplc|scadabr"
```

## File Structure

```
indra-ctf/
├── deployment/
│   ├── docker-compose-ctf-final.yml  # Main config
│   ├── deploy.sh                     # Deploy script
│   ├── stop.sh                       # Stop script
│   ├── restart.sh                    # Restart script
│   ├── status.sh                     # Status script
│   └── README.md                     # Deployment docs
│
├── event-management/
│   ├── ctf-admin.sh                  # Admin tool (UPDATED)
│   ├── status.sh                     # Status (UPDATED)
│   ├── monitor.sh                    # Monitor (UPDATED)
│   ├── SCADA-INTEGRATION.md          # SCADA guide
│   └── ... (other scripts)
│
├── src/                              # Source code
├── docker/                           # Dockerfiles
├── DEPLOYMENT-GUIDE.md               # This file
└── UPDATED-FEATURES.md               # Changelog
```

## Next Steps

1. **Deploy**: `cd deployment && ./deploy.sh`
2. **Verify**: `./status.sh`
3. **Access**: Open http://localhost:9001
4. **Monitor**: `cd ../event-management && ./monitor.sh`
5. **Manage**: `./ctf-admin.sh`

## Support

- Deployment issues: Check `deployment/README.md`
- SCADA integration: Check `event-management/SCADA-INTEGRATION.md`
- Updates: Check `UPDATED-FEATURES.md`
- Management: Use `event-management/ctf-admin.sh`
