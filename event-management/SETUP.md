# Event Management Setup

## Initial Setup on VM

1. **Copy scripts to VM:**
   ```bash
   # From your local machine
   scp -r event-management/ svresidency_kovai@34.180.32.44:~/
   ```

2. **On the VM, make scripts executable:**
   ```bash
   cd ~/event-management
   chmod +x make-executable.sh
   ./make-executable.sh
   ```

3. **Test the setup:**
   ```bash
   ./status.sh
   ```

4. **Launch interactive manager:**
   ```bash
   ./start.sh
   ```

## Pre-Event Preparation

### 1 Day Before Event

```bash
# Create backup
./backup.sh

# Verify all systems
./status.sh
./check-flags.sh

# Test each team
for i in {1..5}; do
    echo "Testing team $i..."
    ./network-check.sh $(printf "%03d" $i)
done
```

### Event Day Morning

**Option 1: Using Interactive Menu (Recommended)**
```bash
# Launch the interactive manager
./start.sh

# Use menu options:
# - Option 2: Restart all containers
# - Option 1: Check status
# - Option 8: Start live monitoring
```

**Option 2: Using Command Line**
```bash
# Restart all containers for fresh start
./restart-all.sh

# Verify everything is running
./status.sh

# Start monitoring in separate terminal
./monitor.sh
```

## During Event

### Keep These Terminals Open

**Terminal 1: Monitoring**
```bash
./monitor.sh
```

**Terminal 2: Ready for commands**
```bash
cd ~/event-management
# Ready to run any script as needed
```

**Terminal 3: Logs (if needed)**
```bash
# Use as needed for troubleshooting
```

## Common Event Scenarios

### Scenario 1: Team says "nothing is working"

```bash
# Step 1: Check their containers
./status.sh team001

# Step 2: Check logs
./logs.sh team001

# Step 3: Restart if needed
./restart-team.sh 001

# Step 4: Verify network
./network-check.sh 001
```

### Scenario 2: Breaker stuck in one position

```bash
# Reset just the breaker
./reset-breaker.sh 001 v1
```

### Scenario 3: Multiple teams having issues

```bash
# Check overall status
./status.sh

# If widespread issue, restart all
./restart-all.sh
```

### Scenario 4: Need to check if flags are accessible

```bash
# Check all teams
./check-flags.sh

# Check specific team
./check-flags.sh 001
```

## Emergency Procedures

### Complete System Failure

```bash
# 1. Stop everything
./emergency-stop.sh

# 2. Check system resources
docker stats
df -h
free -h

# 3. Restart
./restart-all.sh

# 4. Verify
./status.sh
```

### Individual Team Reset

```bash
# Full reset (removes and recreates containers)
./reset-team.sh 001
```

## Post-Event

```bash
# 1. Create final backup
./backup.sh

# 2. Export all logs
mkdir -p /tmp/event-logs
for i in {1..5}; do
    TEAM=$(printf "team%03d" $i)
    ./logs.sh $TEAM > /tmp/event-logs/${TEAM}_logs.txt 2>&1
done

# 3. Stop all containers
./emergency-stop.sh

# 4. Archive backups
tar -czf ctf-event-$(date +%Y%m%d).tar.gz /home/svresidency_kovai/ctf-backups/
```

## Troubleshooting the Scripts

### Scripts not executable
```bash
chmod +x *.sh
```

### Script can't find containers
```bash
# Verify containers exist
docker ps -a | grep team

# Check naming convention matches
docker ps --format "{{.Names}}" | grep team
```

### Permission denied
```bash
# Run with sudo if needed
sudo ./script.sh
```

## Tips

- Keep `./status.sh` running every 15-30 minutes
- Use `./monitor.sh` for real-time overview
- Always check logs before restarting: `./logs.sh <container>`
- Create backups before major changes: `./backup.sh`
- Document any issues in a separate log file

## Contact Info During Event

- VM IP: 34.180.32.44
- VM User: svresidency_kovai
- Scripts Location: ~/event-management/
- Backups Location: ~/ctf-backups/
