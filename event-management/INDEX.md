# Event Management Scripts - Complete Index

## 🎮 Interactive Interface (START HERE!)

| File | Description |
|------|-------------|
| **start.sh** | 🚀 **MAIN LAUNCHER** - Start here! |
| **ctf-manager.sh** | Interactive management console with menus |
| **QUICK-START.md** | One-page quick reference for the menu |
| **INTERACTIVE-GUIDE.md** | Complete guide with examples and workflows |

## 📚 Documentation

| File | Description |
|------|-------------|
| **README.md** | Overview of all scripts and features |
| **SETUP.md** | Initial setup and event day procedures |
| **quick-reference.md** | Command-line troubleshooting guide |
| **INDEX.md** | This file - complete file listing |

## 🛠️ Management Scripts

### Core Operations
| Script | Purpose | Example |
|--------|---------|---------|
| **status.sh** | Check container status | `./status.sh team001` |
| **restart-all.sh** | Restart all containers | `./restart-all.sh` |
| **restart-team.sh** | Restart specific team | `./restart-team.sh 001` |
| **reset-team.sh** | Full team reset | `./reset-team.sh 001` |
| **reset-breaker.sh** | Reset specific breaker | `./reset-breaker.sh 001 v1` |

### Monitoring & Diagnostics
| Script | Purpose | Example |
|--------|---------|---------|
| **logs.sh** | View container logs | `./logs.sh team001-breaker-v1` |
| **monitor.sh** | Real-time dashboard | `./monitor.sh` |
| **check-flags.sh** | Verify flag accessibility | `./check-flags.sh 001` |
| **network-check.sh** | Test connectivity | `./network-check.sh 001` |

### Emergency & Maintenance
| Script | Purpose | Example |
|--------|---------|---------|
| **emergency-stop.sh** | Stop all containers | `./emergency-stop.sh` |
| **backup.sh** | Create backup | `./backup.sh` |

## 🔧 Utility Scripts

| File | Purpose |
|------|---------|
| **make-executable.sh** | Make all scripts executable |

## 📖 How to Use

### For Event Day (Recommended)
```bash
# 1. Launch interactive menu
./start.sh

# 2. Use menu options for all tasks
# - No commands to remember
# - Guided workflows
# - Color-coded feedback
```

### For Advanced Users
```bash
# Use individual scripts directly
./status.sh
./restart-team.sh 001
./logs.sh team001-breaker-v1
```

## 🎯 Quick Navigation

**New to the system?**
1. Read `QUICK-START.md` (2 minutes)
2. Run `./start.sh`
3. Explore the menu

**Setting up for event?**
1. Read `SETUP.md`
2. Run `./make-executable.sh`
3. Test with `./start.sh`

**Need troubleshooting?**
1. Check `quick-reference.md`
2. Use menu option 3 (View Logs)
3. Use menu option 4 (Check Network)

**During the event?**
1. Keep `./start.sh` running
2. Use option 8 for monitoring
3. Refer to `QUICK-START.md` for common scenarios

## 📁 File Organization

```
event-management/
├── start.sh                    ⭐ START HERE
├── ctf-manager.sh              🎮 Interactive menu
├── QUICK-START.md              📋 Quick reference
├── INTERACTIVE-GUIDE.md        📖 Detailed guide
├── README.md                   📚 Overview
├── SETUP.md                    🔧 Setup instructions
├── quick-reference.md          🆘 Troubleshooting
├── INDEX.md                    📑 This file
├── make-executable.sh          🔨 Setup utility
├── status.sh                   📊 Status check
├── restart-all.sh              🔄 Restart all
├── restart-team.sh             🔄 Restart team
├── reset-team.sh               ♻️  Reset team
├── reset-breaker.sh            ♻️  Reset breaker
├── logs.sh                     📝 View logs
├── monitor.sh                  📺 Live monitor
├── check-flags.sh              🚩 Check flags
├── network-check.sh            🌐 Network test
├── emergency-stop.sh           🛑 Emergency stop
└── backup.sh                   💾 Create backup
```

## 🎨 Features

### Interactive Menu
- ✅ Easy team selection (1-5 or All)
- ✅ Component selection (breaker-v1, breaker-v2, control, kali)
- ✅ Color-coded output
- ✅ Guided workflows
- ✅ No command syntax to remember
- ✅ Built-in help and navigation

### Individual Scripts
- ✅ Can be used standalone
- ✅ Scriptable for automation
- ✅ Detailed output
- ✅ Error handling

## 🔗 Related Files

- **Deployment**: `../deployment/deploy-cloud-hardened.sh`
- **Docker Compose**: `../deployment/docker-compose-ctf-final.yml`
- **Challenge Docs**: `../CHALLENGE-*.md`

## 📞 Support

For issues during the event:
1. Check logs: Menu → 3 → Select team/component
2. Try restart: Menu → 2 → Select team/component
3. Check network: Menu → 4 → Select team
4. Refer to `quick-reference.md` for specific errors

---

**Last Updated**: 2024-11-21  
**Version**: 1.0  
**Maintainer**: CTF Admin Team
