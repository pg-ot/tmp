# Deploy to VM Instructions

## From Your Local Windows Machine

### Option 1: Using SCP (Recommended)

```powershell
# From your local machine (PowerShell)
cd C:\Users\z003y2hc\Documents\GitHub\indra

# Copy entire event-management folder to VM
scp -r event-management svresidency_kovai@34.180.32.44:~/
```

### Option 2: Using Git (if repo is on GitHub)

```bash
# SSH to VM first
ssh svresidency_kovai@34.180.32.44

# On VM
cd ~
git clone https://github.com/your-repo/indra.git
# Or if already cloned:
cd ~/indra
git pull
```

### Option 3: Using rsync (if available)

```bash
rsync -avz -e ssh event-management/ svresidency_kovai@34.180.32.44:~/event-management/
```

## On the VM (After Copy)

```bash
# SSH to VM
ssh svresidency_kovai@34.180.32.44

# Navigate to folder
cd ~/event-management

# Make scripts executable
chmod +x make-executable.sh
./make-executable.sh

# Test
./start.sh
```

## Quick One-Liner

```powershell
# Copy and setup in one go
scp -r event-management svresidency_kovai@34.180.32.44:~/ && ssh svresidency_kovai@34.180.32.44 "cd ~/event-management && chmod +x make-executable.sh && ./make-executable.sh"
```

## Verify Deployment

```bash
# On VM
ls -la ~/event-management/
./start.sh
```
