# CTF Event Management Scripts

Quick reference scripts for managing the IEC 61850 GOOSE CTF environment during the event.

## Quick Start

### Comprehensive Admin Tool (Recommended)

```bash
# Launch full admin tool
./start.sh
```

The admin tool provides:
- **Installation & Setup** - Fresh install, dependencies, submodules, build
- **Deployment** - Deploy teams, start/stop containers
- **Updates & Git** - Pull updates, stash changes, rebuild
- **CTF Management** - Status, logs, monitoring, backups
- **System Info** - Docker versions, resource usage

### Management Only

```bash
# Launch management console only
./start.sh --manage
```

The management console provides:
- Easy team and component selection
- Color-coded status information
- Guided workflows for common tasks
- No need to remember command syntax

### Command Line (Advanced)

```bash
# Check environment status
./status.sh

# Restart all teams
./restart-all.sh

# Reset specific team
./reset-team.sh 001
```

## Available Scripts

### Main Interface

| Script | Purpose |
|--------|---------|
| `start.sh` | Launch interactive menu (RECOMMENDED) |
| `ctf-manager.sh` | Interactive management console |

### Individual Scripts

| Script | Purpose |
|--------|---------|
| `status.sh` | Check status of all containers |
| `restart-all.sh` | Restart all containers |
| `restart-team.sh` | Restart specific team's containers |
| `reset-team.sh` | Full reset of team environment |
| `reset-breaker.sh` | Reset only breaker containers |
| `logs.sh` | View container logs |
| `monitor.sh` | Real-time monitoring dashboard |
| `check-flags.sh` | Verify flag accessibility |
| `network-check.sh` | Verify network connectivity |
| `backup.sh` | Backup current state |
| `emergency-stop.sh` | Stop all containers immediately |

## Emergency Procedures

### Team Reports Issue
```bash
./logs.sh team001
./restart-team.sh 001
```

### Breaker Stuck
```bash
./reset-breaker.sh 001 v1
```

### Full Environment Reset
```bash
./emergency-stop.sh
./restart-all.sh
```

## Monitoring During Event

```bash
# Continuous monitoring
./monitor.sh

# Check specific team
./status.sh team001
```
