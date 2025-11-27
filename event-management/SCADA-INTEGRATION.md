# SCADA Integration Guide

## Overview

The CTF environment now includes OpenPLC and ScadaBR SCADA systems integrated with the IEC 61850 GOOSE network.

## Architecture

### Network Topology

```
┌─────────────────────────────────────────────────────────────┐
│  Mahashakti Network (192.168.100.0/24)                      │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐      │
│  │ Breaker v1   │  │ Breaker v2   │  │ Control IED  │      │
│  │ (Vulnerable) │  │  (Secure)    │  │ (Publisher)  │      │
│  └──────────────┘  └──────────────┘  └──────────────┘      │
│         │                 │                  │              │
│         └─────────────────┴──────────────────┘              │
│                           │                                 │
│                    ┌──────┴──────┐                          │
│                    │ Kali        │                          │
│                    │ Workstation │                          │
│                    └──────┬──────┘                          │
└───────────────────────────┼──────────────────────────────────┘
                            │
┌───────────────────────────┼──────────────────────────────────┐
│  Aushadi Raksha Network (192.168.200.0/24)                  │
│                    ┌──────┴──────┐                          │
│                    │             │                          │
│              ┌─────┴─────┐ ┌────┴─────┐                    │
│              │  OpenPLC  │ │ ScadaBR  │                    │
│              │  Runtime  │ │  SCADA   │                    │
│              └───────────┘ └──────────┘                    │
│              Port 8081      Port 8080                       │
│              Modbus 502                                     │
└─────────────────────────────────────────────────────────────┘
```

## Components

### OpenPLC Runtime
- **Purpose**: PLC simulator with Modbus TCP server
- **Access**: http://localhost:8081
- **Credentials**: openplc / openplc
- **Modbus**: localhost:502
- **Network**: aushadi_raksha (192.168.200.x)

### ScadaBR
- **Purpose**: SCADA HMI system
- **Access**: http://localhost:8080/ScadaBR
- **Credentials**: admin / admin
- **Network**: aushadi_raksha (192.168.200.x)
- **Connects to**: OpenPLC via Modbus TCP

### Kali Workstation
- **Purpose**: Attacker workstation with dual network access
- **Networks**: 
  - eth0: mahashakti (192.168.100.x)
  - eth1: aushadi_raksha (192.168.200.x)
- **Can access**: All IEDs, OpenPLC, ScadaBR

## Management Commands

### Start Environment
```bash
cd deployment
docker-compose -f docker-compose-ctf-final.yml up -d
```

### Check Status
```bash
cd event-management
./status.sh
```

### Monitor Live
```bash
cd event-management
./monitor.sh
```

### Stop All
```bash
cd event-management
./ctf-admin.sh
# Select: Deployment > Stop All Containers
```

## Access URLs

From host machine:
- **Breaker v1**: http://localhost:9001
- **Breaker v2**: http://localhost:9002
- **OpenPLC**: http://localhost:8081
- **ScadaBR**: http://localhost:8080/ScadaBR
- **Modbus TCP**: localhost:502

From Kali container:
```bash
docker exec -it kali-workstation bash

# Access SCADA systems
curl http://openplc:8080
curl http://scadabr:8080

# Access IEDs
curl http://breaker-v1:9000
curl http://breaker-v2:9000

# Modbus communication
python3 -c "from pymodbus.client import ModbusTcpClient; c = ModbusTcpClient('openplc', port=502); c.connect()"
```

## Updated Scripts

The following scripts have been updated to support OpenPLC and ScadaBR:

1. **ctf-admin.sh**
   - Deploy shows SCADA URLs
   - Stop/Start includes SCADA containers
   - Cleanup removes SCADA images
   - System info shows SCADA images

2. **status.sh**
   - Shows SCADA container status
   - Separate section for SCADA systems

3. **monitor.sh**
   - Monitors SCADA container health
   - Shows SCADA in container count

## Troubleshooting

### SCADA containers not starting
```bash
# Check logs
docker logs openplc
docker logs scadabr

# Restart SCADA systems
docker restart openplc scadabr
```

### Kali can't access SCADA
```bash
# Verify Kali is on both networks
docker exec kali-workstation ip addr show

# Should see eth0 (mahashakti) and eth1 (aushadi_raksha)
```

### Port conflicts
If ports 8080, 8081, or 502 are in use, edit `docker-compose-ctf-final.yml`:
```yaml
ports:
  - "9080:8080"  # Change host port
  - "9081:8080"  # Change host port
  - "5502:502"   # Change host port
```

## Challenge Integration

Participants can now:
1. Attack IEDs via GOOSE protocol (original challenge)
2. Manipulate PLC via Modbus TCP
3. Monitor/control via ScadaBR HMI
4. Pivot between OT networks

This creates a more realistic industrial environment with multiple attack vectors.
