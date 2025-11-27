# Quick Reference - Event Day

## Common Issues & Solutions

### Issue: Team reports "breaker won't trip"

```bash
# Check breaker status
./logs.sh team001-breaker-v1

# Reset breaker
./reset-breaker.sh 001 v1

# Check network
./network-check.sh 001
```

### Issue: Container not responding

```bash
# Check status
./status.sh team001

# Restart team
./restart-team.sh 001

# View logs
./logs.sh team001
```

### Issue: Kali workstation SSH not working

```bash
# Check if container is running
docker ps | grep team001-kali

# Restart Kali
docker restart team001-kali

# Check SSH port
docker port team001-kali
```

### Issue: No GOOSE traffic visible

```bash
# Check control IED
docker exec team001-control ps aux

# Restart control IED
docker restart team001-control

# Verify from Kali
docker exec team001-kali tcpdump -i eth0 ether proto 0x88b8 -c 5
```

## Pre-Event Checklist

```bash
# 1. Check all containers
./status.sh

# 2. Verify flags
./check-flags.sh

# 3. Test network
for i in {1..5}; do ./network-check.sh $(printf "%03d" $i); done

# 4. Create backup
./backup.sh

# 5. Start monitoring
./monitor.sh
```

## During Event

### Every 30 minutes
```bash
./status.sh
```

### If issues arise
```bash
./logs.sh <team_or_container>
./restart-team.sh <team_number>
```

## Post-Event

```bash
# Final backup
./backup.sh

# Stop all
./emergency-stop.sh

# Export logs
for i in {1..5}; do
    TEAM=$(printf "team%03d" $i)
    ./logs.sh $TEAM > /tmp/${TEAM}_final_logs.txt
done
```

## SSH Access Info

| Team | SSH Port | Command |
|------|----------|---------|
| 001  | 20001    | `ssh ctfuser@<VM_IP> -p 20001` |
| 002  | 20002    | `ssh ctfuser@<VM_IP> -p 20002` |
| 003  | 20003    | `ssh ctfuser@<VM_IP> -p 20003` |
| 004  | 20004    | `ssh ctfuser@<VM_IP> -p 20004` |
| 005  | 20005    | `ssh ctfuser@<VM_IP> -p 20005` |

Password: `IEC61850_CTF_2024`

## Container Names

Format: `team<NNN>-<component>`

Components:
- `breaker-v1` - Vulnerable breaker
- `breaker-v2` - Secure breaker
- `control` - Control IED (GOOSE publisher)
- `kali` - Kali workstation

Example: `team001-breaker-v1`

## Useful Docker Commands

```bash
# View all team containers
docker ps --filter "name=team"

# Restart specific container
docker restart team001-breaker-v1

# View logs
docker logs -f team001-breaker-v1

# Execute command in container
docker exec team001-kali <command>

# Get container IP
docker inspect team001-breaker-v1 | grep IPAddress

# Check resource usage
docker stats --no-stream
```
