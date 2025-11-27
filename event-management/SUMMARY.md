# Event Management Scripts - Summary

## ✅ What's Been Created

A complete interactive management system for your IEC 61850 GOOSE CTF event with:

### 🎮 Interactive Interface
- **Main launcher** (`start.sh`) - One command to rule them all
- **Interactive menu** (`ctf-manager.sh`) - Color-coded, guided workflows
- **No commands to remember** - Just select options from menus

### 📊 Management Capabilities
- ✅ Check status of all teams/containers
- ✅ Restart individual containers or entire teams
- ✅ View real-time logs
- ✅ Monitor network connectivity
- ✅ Verify flag accessibility
- ✅ Access container shells
- ✅ Live monitoring dashboard
- ✅ Create backups
- ✅ Emergency stop all

### 📚 Complete Documentation
- Quick start guide (2-minute read)
- Interactive guide with examples
- Setup instructions
- Troubleshooting reference
- Complete file index

## 🚀 Getting Started

### On VM (One-Time Setup)

```bash
# 1. Copy files to VM
scp -r event-management/ svresidency_kovai@34.180.32.44:~/

# 2. SSH to VM
ssh svresidency_kovai@34.180.32.44

# 3. Setup
cd ~/event-management
chmod +x make-executable.sh
./make-executable.sh

# 4. Launch!
./start.sh
```

### During Event

```bash
# Just run this:
cd ~/event-management
./start.sh

# Then use the menu for everything!
```

## 📋 Menu Overview

```
Main Menu:
  1) Show Status           ← Check if everything is running
  2) Restart Containers    ← Fix issues by restarting
  3) View Logs            ← See what's happening
  4) Check Network        ← Verify connectivity
  5) Check Flags          ← Ensure challenges work
  6) Reset Team           ← Full team reset
  7) Access Shell         ← Run commands in containers
  8) Live Monitor         ← Real-time dashboard
  9) Create Backup        ← Save current state
  0) Emergency Stop       ← Stop everything
  q) Quit
```

## 🎯 Common Event Scenarios

### Team Reports Issue
```
Menu → 3 (Logs) → Select Team → Select Component
Menu → 2 (Restart) → Select Team → Select Component
```

### Check All Teams
```
Menu → 1 (Status) → Select "All Teams"
```

### Monitor Continuously
```
Menu → 8 (Live Monitor)
```

### Access Kali Container
```
Menu → 7 (Access Shell) → Select Team → Select "Kali"
```

## 📁 File Structure

```
event-management/
├── 🚀 start.sh                 ← START HERE!
├── 🎮 ctf-manager.sh           ← Interactive menu
│
├── 📖 Documentation
│   ├── QUICK-START.md          ← 2-minute guide
│   ├── INTERACTIVE-GUIDE.md    ← Complete guide
│   ├── SETUP.md                ← Setup instructions
│   ├── quick-reference.md      ← Troubleshooting
│   ├── INDEX.md                ← File listing
│   └── README.md               ← Overview
│
└── 🛠️ Individual Scripts (can also be used standalone)
    ├── status.sh
    ├── restart-all.sh
    ├── restart-team.sh
    ├── reset-team.sh
    ├── reset-breaker.sh
    ├── logs.sh
    ├── monitor.sh
    ├── check-flags.sh
    ├── network-check.sh
    ├── emergency-stop.sh
    └── backup.sh
```

## 💡 Key Features

### Interactive Menu Benefits
- ✅ **No memorization** - All options visible
- ✅ **Guided selection** - Choose team, then component
- ✅ **Color-coded** - Green=good, Red=error, Yellow=warning
- ✅ **Safe** - Confirmation prompts for destructive actions
- ✅ **Quick stats** - See running/stopped containers at a glance
- ✅ **Easy navigation** - Type 'b' to go back, 'q' to quit

### Flexibility
- ✅ Use interactive menu OR individual scripts
- ✅ Works for single team or all teams
- ✅ Can target specific components
- ✅ Scriptable for automation

## 🎓 Learning Curve

**Beginner**: 5 minutes
- Read `QUICK-START.md`
- Run `./start.sh`
- Explore menu options

**Intermediate**: 15 minutes
- Read `INTERACTIVE-GUIDE.md`
- Practice common workflows
- Try individual scripts

**Advanced**: 30 minutes
- Read all documentation
- Understand script internals
- Customize for your needs

## 🔧 Customization

All scripts are well-commented and easy to modify:
- Adjust team numbers in `ctf-manager.sh`
- Modify colors in menu
- Add custom checks
- Extend functionality

## 📞 Quick Reference Card

**Print this for event day:**

| Issue | Solution |
|-------|----------|
| Team can't connect | Menu → 4 (Network) → Select team |
| Breaker stuck | Menu → 2 (Restart) → Team → Breaker |
| Need to see errors | Menu → 3 (Logs) → Team → Component |
| Check all teams | Menu → 1 (Status) → All Teams |
| Monitor event | Menu → 8 (Live Monitor) |
| Before changes | Menu → 9 (Backup) |
| Emergency | Menu → 0 (Stop All) |

## ✨ What Makes This Special

1. **Interactive** - No command syntax to remember
2. **Visual** - Color-coded, clear output
3. **Safe** - Confirmations for destructive actions
4. **Flexible** - Menu OR command-line
5. **Complete** - Everything you need in one place
6. **Documented** - Multiple guides for different needs
7. **Tested** - Ready for production use

## 🎉 You're Ready!

Everything is set up for a smooth CTF event. Just:

1. Copy to VM
2. Run `./make-executable.sh`
3. Launch with `./start.sh`
4. Use the menu for everything!

**Good luck with your CTF event! 🚀**

---

**Questions?** Check `INDEX.md` for complete file listing  
**Need help?** See `quick-reference.md` for troubleshooting  
**First time?** Start with `QUICK-START.md`
