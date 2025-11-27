# Quick Start - Interactive Menu

## 🚀 Launch

```bash
cd ~/event-management
./start.sh
```

## 📋 Menu Options

| Key | Action | Use When |
|-----|--------|----------|
| `1` | Show Status | Check if containers are running |
| `2` | Restart Containers | Team reports issues |
| `3` | View Logs | Troubleshoot problems |
| `4` | Check Network | Verify connectivity |
| `5` | Check Flags | Verify challenges work |
| `6` | Reset Team | Full team environment reset |
| `7` | Access Shell | Need to run commands in container |
| `8` | Live Monitor | Continuous monitoring |
| `9` | Create Backup | Before major changes |
| `0` | Emergency Stop | Critical issues |
| `q` | Quit | Exit the menu |

## 🎯 Common Scenarios

### Team Says "Nothing Works"
```
Menu → 1 (Status) → Select Team → Check status
Menu → 3 (Logs) → Select Team → Select Component → View errors
Menu → 2 (Restart) → Select Team → Select "All Components"
```

### Breaker Stuck
```
Menu → 2 (Restart) → Select Team → Select "Breaker v1" or "Breaker v2"
```

### Check All Teams
```
Menu → 1 (Status) → Select "All Teams"
```

### Monitor During Event
```
Menu → 8 (Live Monitor)
[Runs continuously, press Ctrl+C to exit]
```

### Access Kali Container
```
Menu → 7 (Access Shell) → Select Team → Select "Kali Workstation"
[Type 'exit' to return]
```

## 💡 Tips

- **Keep menu open** in a dedicated terminal
- **Use Live Monitor** (option 8) during event
- **Check logs first** before restarting
- **Create backups** before major changes
- Press **Ctrl+C** to interrupt any operation
- Type **b** to go back in menus
- Type **q** to quit

## 🆘 Emergency

If menu is not working:
```bash
# Direct commands
./status.sh
./restart-all.sh
./emergency-stop.sh
```

## 📞 Quick Reference

- **Teams**: 001, 002, 003, 004, 005
- **Components**: breaker-v1, breaker-v2, control, kali
- **SSH Ports**: 20001-20005
- **Password**: IEC61850_CTF_2024

---

**For detailed guide**: See `INTERACTIVE-GUIDE.md`  
**For troubleshooting**: See `quick-reference.md`
