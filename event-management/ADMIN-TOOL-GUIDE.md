# CTF Admin Tool Guide

Comprehensive administration tool for the IEC 61850 GOOSE CTF challenge.

## Quick Start

```bash
cd ~/indra/event-management
./start.sh
```

## Features

### 1. Installation & Setup
- Fresh installation (clone repo)
- Install dependencies (Docker, Docker Compose, Git)
- Initialize Git submodules
- Build Docker images
- Complete automated setup

### 2. Deployment
- Deploy CTF environment
- Deploy specific number of teams
- Stop/remove containers
- Full cleanup

### 3. Updates & Git
- Check for updates
- Pull latest changes
- Stash local changes
- Update and rebuild
- View Git status/log

### 4. CTF Management
- Status monitoring
- Container restart
- Log viewing
- Network checks
- Flag verification
- Team reset
- Container shell access
- Live monitoring
- Backup creation

### 5. System Information
- Docker version
- Disk/memory usage
- Image listing

## Menu Structure

```
Main Menu
├── 1) Installation & Setup
│   ├── Fresh Installation
│   ├── Install Dependencies
│   ├── Initialize Submodules
│   ├── Build Images
│   └── Complete Setup
│
├── 2) Deployment
│   ├── Deploy CTF Environment
│   ├── Deploy Multiple Teams
│   ├── Stop All
│   ├── Remove All
│   └── Full Cleanup
│
├── 3) Updates & Git
│   ├── Check for Updates
│   ├── Pull Latest
│   ├── Stash Changes
│   ├── Update & Rebuild
│   ├── Git Status
│   └── Git Log
│
├── 4) CTF Management
│   └── [Launches ctf-manager.sh]
│
└── 5) System Information
```

## Usage Scenarios

### First Time Setup

```bash
# 1. Launch admin tool
./start.sh

# 2. Select: 1) Installation & Setup
# 3. Select: 5) Complete Setup
# This will install everything automatically
```

### Daily Operations

```bash
# Launch admin tool
./start.sh

# Select: 4) CTF Management
# Use for monitoring and managing running CTF
```

### Updating from Git

```bash
# Launch admin tool
./start.sh

# Select: 3) Updates & Git
# Select: 4) Update and Rebuild
# This stashes changes, pulls updates, and rebuilds
```

### Deploying for Event

```bash
# Launch admin tool
./start.sh

# Select: 2) Deployment
# Select: 2) Deploy Specific Number of Teams
# Enter number of teams (e.g., 5)
```

## Command Line Options

```bash
# Launch full admin tool
./start.sh

# Launch management only (skip installation/deployment menus)
./start.sh --manage
# or
./start.sh -m
```

## Configuration

Edit `ctf-admin.sh` to customize:

```bash
# Repository URL
REPO_URL="https://github.com/your-username/indra.git"

# Installation directory
INSTALL_DIR="$HOME/indra"

# Docker Compose file
COMPOSE_FILE="deployment/docker-compose-ctf-final.yml"
```

## Features in Detail

### Installation & Setup

**Fresh Installation:**
- Clones repository with submodules
- Sets up directory structure
- Provides next steps

**Install Dependencies:**
- Checks for Docker, Docker Compose, Git
- Installs missing dependencies
- Adds user to docker group

**Initialize Submodules:**
- Checks submodule status
- Initializes libiec61850
- Handles errors gracefully

**Build Images:**
- Builds all Docker images
- Shows progress
- Reports success/failure

**Complete Setup:**
- Runs all above steps in sequence
- One-click setup for new installations

### Deployment

**Deploy CTF Environment:**
- Starts docker-compose
- Shows access URLs
- Verifies deployment

**Deploy Multiple Teams:**
- Uses deploy-cloud-hardened.sh
- Supports 1-100+ teams
- Isolated networks per team

**Stop/Remove/Cleanup:**
- Graceful shutdown
- Container removal
- Full cleanup (images + networks)
- Confirmation prompts for destructive actions

### Updates & Git

**Check for Updates:**
- Fetches from remote
- Compares local vs remote
- Shows available updates

**Pull Latest:**
- Pulls from Git
- Updates submodules
- Handles merge conflicts

**Stash Changes:**
- Saves local modifications
- Timestamped stash messages
- Allows clean pulls

**Update & Rebuild:**
- Complete update workflow
- Stash → Pull → Submodules → Build
- One-click update process

**Git Status/Log:**
- View repository status
- See recent commits
- Check for uncommitted changes

### CTF Management

Launches the existing `ctf-manager.sh` with all management features:
- Status monitoring
- Container operations
- Log viewing
- Network diagnostics
- Flag verification
- Backup creation

### System Information

- Docker/Compose versions
- System resources (disk, memory)
- Docker image listing
- Quick health check

## Troubleshooting

### "Not in a Git repository"

**Solution:**
```bash
cd ~/indra/event-management
./start.sh
```

### "Permission denied"

**Solution:**
```bash
chmod +x *.sh
./start.sh
```

### "Docker command not found"

**Solution:**
```bash
# Use Installation menu
./start.sh
# Select: 1) Installation & Setup
# Select: 2) Install Dependencies
```

### Submodule not initialized

**Solution:**
```bash
# Use Installation menu
./start.sh
# Select: 1) Installation & Setup
# Select: 3) Initialize Git Submodules
```

## Best Practices

### Before Event

1. Run complete setup
2. Test deployment with 1 team
3. Create backup
4. Deploy all teams
5. Verify all containers running

### During Event

1. Keep admin tool open in terminal
2. Use CTF Management for monitoring
3. Check status every 30 minutes
4. Create periodic backups

### After Event

1. Create final backup
2. Export logs
3. Stop all containers
4. Optional: Full cleanup

## Integration with Existing Scripts

The admin tool integrates with:
- `ctf-manager.sh` - Management console
- `deploy-cloud-hardened.sh` - Team deployment
- `build-containers.sh` - Image building
- All individual management scripts

## Automation

### Cron Job for Monitoring

```bash
# Add to crontab
*/30 * * * * cd ~/indra/event-management && ./status.sh >> /var/log/ctf-status.log
```

### Automated Backups

```bash
# Daily backup at 2 AM
0 2 * * * cd ~/indra/event-management && ./backup.sh
```

## Security Notes

- Admin tool requires appropriate permissions
- Destructive actions require confirmation
- Git operations preserve local changes (stash)
- Backup before major operations

## Future Enhancements

Planned features:
- Automated health checks
- Email notifications
- Metrics dashboard
- Team-specific operations
- Bulk operations
- Configuration wizard

---

**Version:** 1.0  
**Last Updated:** 2024-11-22  
**Compatibility:** Ubuntu 20.04+, Docker 20.10+
