# Interactive Menu Guide

## Starting the Interactive Manager

```bash
cd ~/event-management
./start.sh
```

## Main Menu

```
╔════════════════════════════════════════════════════════════════╗
║         IEC 61850 GOOSE CTF - Management Console              ║
║         2024-11-21 15:30:45                                    ║
╚════════════════════════════════════════════════════════════════╝

Quick Stats:
  Running: 20 / 20
  
Main Menu:
  1) Show Status
  2) Restart Containers
  3) View Logs
  4) Check Network
  5) Check Flags
  6) Reset Team
  7) Access Container Shell
  8) Live Monitor
  9) Create Backup
  0) Emergency Stop All
  q) Quit

Choice: _
```

## Common Workflows

### Workflow 1: Check Team Status

1. Select option `1` (Show Status)
2. Select team (1-5) or `a` for all teams
3. View status information
4. Press Enter to return to menu

**Example:**
```
Choice: 1

Select Team:
  1) Team 001
  2) Team 002
  3) Team 003
  4) Team 004
  5) Team 005
  a) All Teams
  b) Back

Choice: 1

Team: team001
NAMES                    STATUS              PORTS
team001-kali            Up 2 hours          0.0.0.0:20001->22/tcp
team001-control         Up 2 hours          
team001-breaker-v2      Up 2 hours          
team001-breaker-v1      Up 2 hours          

Press Enter to continue...
```

### Workflow 2: Restart Specific Container

1. Select option `2` (Restart Containers)
2. Select team (1-5)
3. Select component (breaker-v1, breaker-v2, control, kali, or all)
4. Confirm restart
5. Press Enter to return to menu

**Example:**
```
Choice: 2

Select Team:
  1) Team 001
  ...
Choice: 1

Select Component:
  1) Breaker v1
  2) Breaker v2
  3) Control IED
  4) Kali Workstation
  5) All Components
  b) Back

Choice: 1

Restarting team001-breaker-v1...
✓ Restarted

Press Enter to continue...
```

### Workflow 3: View Live Logs

1. Select option `3` (View Logs)
2. Select team
3. Select component
4. Watch logs in real-time
5. Press Ctrl+C to stop
6. Press Enter to return to menu

**Example:**
```
Choice: 3

Select Team:
  1) Team 001
  ...
Choice: 1

Select Component:
  1) Breaker v1
  ...
Choice: 1

Viewing logs for team001-breaker-v1
Press Ctrl+C to exit

[2024-11-21 15:30:45] Breaker IED v1 started
[2024-11-21 15:30:46] Listening for GOOSE messages...
[2024-11-21 15:30:50] GOOSE message received
^C

Press Enter to continue...
```

### Workflow 4: Access Container Shell

1. Select option `7` (Access Container Shell)
2. Select team
3. Select component
4. You're now inside the container
5. Type `exit` to return to menu

**Example:**
```
Choice: 7

Select Team:
  1) Team 001
  ...
Choice: 1

Select Component:
  1) Breaker v1
  2) Breaker v2
  3) Control IED
  4) Kali Workstation
  ...
Choice: 4

Accessing team001-kali...
Type 'exit' to return to menu

root@team001-kali:/# tcpdump -i eth0 ether proto 0x88b8 -c 5
...
root@team001-kali:/# exit

[Back to main menu]
```

### Workflow 5: Check Network Connectivity

1. Select option `4` (Check Network)
2. Select team
3. View connectivity test results
4. Press Enter to return to menu

**Example:**
```
Choice: 4

Select Team:
  1) Team 001
  ...
Choice: 1

=== Network Connectivity Check: Team 001 ===

Testing from Kali workstation...

1. Ping breaker-v1:
   ✓ Success

2. Ping breaker-v2:
   ✓ Success

3. Ping control IED:
   ✓ Success

4. Check GOOSE traffic:
   ✓ GOOSE packets detected (5 packets)

5. HTTP access to breaker-v1:
   ✓ Success

✓ Network check complete

Press Enter to continue...
```

### Workflow 6: Live Monitoring

1. Select option `8` (Live Monitor)
2. Watch real-time dashboard
3. Press Ctrl+C to return to menu

**Example:**
```
Choice: 8

╔════════════════════════════════════════════════════════════════╗
║         IEC 61850 GOOSE CTF - Live Monitor                    ║
║         2024-11-21 15:30:45                                    ║
╚════════════════════════════════════════════════════════════════╝

📊 Container Status:
   Running: 20 / 20

💻 System Resources:
   CPU: 15.2% used
   Memory: 4.2G / 16G
   Disk: 45G / 100G (45% used)

👥 Team Status:
   ✓ Team 1: 4/4 containers
   ✓ Team 2: 4/4 containers
   ✓ Team 3: 4/4 containers
   ✓ Team 4: 4/4 containers
   ✓ Team 5: 4/4 containers

🔄 Refreshing in 5 seconds...
```

## Tips for Event Day

### Keep Menu Open
- Run in a dedicated terminal
- Quick access to all functions
- No need to remember commands

### Common Actions
- **Team reports issue**: Option 3 (Logs) → Option 2 (Restart)
- **Check all teams**: Option 1 (Status) → Select "All Teams"
- **Monitor continuously**: Option 8 (Live Monitor)
- **Quick flag check**: Option 5 (Check Flags)

### Navigation
- Type number and press Enter
- Type `b` to go back
- Type `q` to quit
- Press Ctrl+C to interrupt long-running operations

### Color Coding
- 🟢 **Green**: Success, running
- 🔴 **Red**: Error, stopped, warning
- 🟡 **Yellow**: Action required, prompts
- 🔵 **Blue**: Information
- 🟦 **Cyan**: Headers, titles

## Keyboard Shortcuts

- **Enter**: Confirm selection / Continue
- **Ctrl+C**: Stop current operation / Exit logs
- **q**: Quit application
- **b**: Go back to previous menu

## Troubleshooting the Menu

### Menu doesn't start
```bash
chmod +x start.sh ctf-manager.sh
./start.sh
```

### Colors not showing
- Your terminal may not support colors
- Scripts will still work, just without colors

### Script not found errors
```bash
# Make sure you're in the right directory
cd ~/event-management

# Make all scripts executable
chmod +x *.sh
```

## Advanced: Running Without Interactive Menu

If you prefer command line:

```bash
# Direct commands
./status.sh team001
./restart-team.sh 001
./logs.sh team001-breaker-v1
./network-check.sh 001
```

See `quick-reference.md` for all command-line options.
