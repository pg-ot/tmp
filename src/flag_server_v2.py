#!/usr/bin/env python3
import http.server
import socketserver
import json
import os
from pathlib import Path

PORT = 9000
STATUS_FILE = '/tmp/breaker_status.json'
MAX_FILE_SIZE = 10240  # 10KB limit

class BreakerStatusHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        # Prevent path traversal - only allow specific paths
        if self.path not in ['/', '/index.html', '/status']:
            self.send_response(404)
            self.end_headers()
            return
            
        if self.path == '/status':
            try:
                if os.path.exists(STATUS_FILE):
                    # Check file size to prevent memory exhaustion
                    if os.path.getsize(STATUS_FILE) > MAX_FILE_SIZE:
                        raise ValueError("Status file too large")
                    
                    with open(STATUS_FILE, 'r') as f:
                        data = json.load(f)
                    
                    # Validate expected fields to prevent injection
                    if not isinstance(data, dict):
                        raise ValueError("Invalid status format")
                    
                    # Sanitize output - only allow expected fields
                    safe_data = {
                        'pos': int(data.get('pos', 0)),
                        'opCnt': int(data.get('opCnt', 0)),
                        'stNum': int(data.get('stNum', 0)),
                        'sqNum': int(data.get('sqNum', 0)),
                        'commStatus': str(data.get('commStatus', 'UNKNOWN'))[:20]
                    }
                    # Only include flag when breaker is open
                    if safe_data['pos'] == 1:
                        safe_data['flag'] = 'FLAG{GOOSE_SEQUENCE_MASTER}'
                else:
                    safe_data = {"error": "Status not available"}
                
                self.send_response(200)
                self.send_header('Content-type', 'application/json')
                self.send_header('Access-Control-Allow-Origin', '*')
                self.send_header('X-Content-Type-Options', 'nosniff')
                self.end_headers()
                self.wfile.write(json.dumps(safe_data).encode())
            except (json.JSONDecodeError, ValueError, KeyError) as e:
                self.send_response(500)
                self.end_headers()
                self.wfile.write(json.dumps({"error": "Invalid data"}).encode())
            except Exception:
                self.send_response(500)
                self.end_headers()
                self.wfile.write(json.dumps({"error": "Server error"}).encode())
        
        elif self.path == '/' or self.path == '/index.html':
            html = """<!DOCTYPE html>
<html>
<head>
    <title>Indraprastha Kingdom - Southern District Grid</title>
    <meta charset="UTF-8">
    <meta http-equiv="refresh" content="2">
    <style>
        body { 
            font-family: 'Segoe UI', Arial, sans-serif; 
            background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%);
            color: #fff;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
            margin: 0;
            padding: 20px;
        }
        .container {
            background: rgba(42, 42, 42, 0.95);
            padding: 30px;
            border-radius: 15px;
            box-shadow: 0 0 30px rgba(255,165,0,0.4);
            text-align: center;
            max-width: 700px;
            border: 2px solid #ffa500;
        }
        .header {
            background: linear-gradient(135deg, #ff8c00 0%, #ffa500 100%);
            padding: 20px;
            border-radius: 10px;
            margin-bottom: 20px;
        }
        h1 { 
            color: #000; 
            margin: 0;
            font-size: 28px;
            text-shadow: 1px 1px 2px rgba(255,255,255,0.3);
        }
        .subtitle {
            color: #333;
            font-size: 14px;
            margin-top: 5px;
            font-weight: 600;
        }
        .mission {
            background: #1a1a1a;
            border-left: 4px solid #ffa500;
            padding: 15px;
            margin: 20px 0;
            text-align: left;
            font-size: 14px;
            line-height: 1.6;
        }
        .mission-title {
            color: #ffa500;
            font-weight: bold;
            margin-bottom: 10px;
        }
        .status {
            font-size: 42px;
            font-weight: bold;
            padding: 25px;
            border-radius: 10px;
            margin: 20px 0;
            text-transform: uppercase;
            letter-spacing: 2px;
        }
        .closed { 
            background: linear-gradient(135deg, #ff0000 0%, #8b0000 100%);
            color: #fff;
            box-shadow: 0 0 20px rgba(255,0,0,0.5);
        }
        .open { 
            background: linear-gradient(135deg, #00ff00 0%, #008000 100%);
            color: #000;
            box-shadow: 0 0 20px rgba(0,255,0,0.5);
        }
        .kingdom-status {
            background: #2a2a2a;
            padding: 15px;
            border-radius: 8px;
            margin: 15px 0;
            border: 1px solid #444;
        }
        .kingdom-status h3 {
            margin: 0 0 10px 0;
            color: #ffa500;
            font-size: 16px;
        }
        .info { 
            background: #1a1a1a; 
            padding: 20px; 
            border-radius: 10px;
            margin-top: 20px;
            text-align: left;
            border: 1px solid #333;
        }
        .info-title {
            color: #ffa500;
            font-weight: bold;
            margin-bottom: 15px;
            font-size: 16px;
            border-bottom: 1px solid #444;
            padding-bottom: 10px;
        }
        .info div { margin: 8px 0; font-size: 14px; }
        .label { 
            color: #888; 
            display: inline-block; 
            width: 180px;
            font-weight: 500;
        }
        .value { color: #fff; font-weight: bold; }
        .flag {
            background: #000;
            border: 3px solid #ffa500;
            color: #ffa500;
            font-family: 'Courier New', monospace;
            font-size: 24px;
            padding: 20px;
            margin: 20px 0;
            animation: blink 1s infinite;
            border-radius: 8px;
            box-shadow: 0 0 20px rgba(255,165,0,0.3);
        }
        @keyframes blink { 50% { opacity: 0.6; } }
        .hidden { display: none; }
        .secure { color: #00ff00; font-weight: bold; }
        .protocol-badge {
            display: inline-block;
            background: #333;
            padding: 5px 10px;
            border-radius: 5px;
            font-size: 12px;
            margin: 5px;
        }
    </style>
    <script>
        async function updateStatus() {
            try {
                const response = await fetch('/status');
                const data = await response.json();
                
                const statusDiv = document.getElementById('status');
                const posDiv = document.getElementById('position');
                const opCntDiv = document.getElementById('opcount');
                const stNumDiv = document.getElementById('stnum');
                const sqNumDiv = document.getElementById('sqnum');
                const flagDiv = document.getElementById('flag');
                const kingdomDiv = document.getElementById('kingdom-msg');
                
                if (data.error) {
                    statusDiv.textContent = 'ERROR';
                    statusDiv.className = 'status';
                } else {
                    const pos = data.pos || 2;
                    
                    if (pos == 2) {
                        statusDiv.textContent = '⚡ POWER ON';
                        statusDiv.className = 'status closed';
                        posDiv.innerHTML = '<span class="value">CLOSED (2)</span>';
                        kingdomDiv.innerHTML = '🏰 <strong>Southern District:</strong> Command centers ACTIVE - Operations coordinated!';
                        flagDiv.className = 'flag hidden';
                    } else if (pos == 1) {
                        statusDiv.textContent = '✓ POWER OFF';
                        statusDiv.className = 'status open';
                        posDiv.innerHTML = '<span class="value">OPEN (1)</span>';
                        kingdomDiv.innerHTML = '🏰 <strong>Southern District:</strong> Command centers DISABLED - Operations disrupted!';
                        flagDiv.className = 'flag';
                        flagDiv.textContent = '🚩 ' + data.flag + ' 🚩';
                    }
                    
                    opCntDiv.innerHTML = '<span class="value">' + (data.opCnt || '0') + '</span>';
                    stNumDiv.innerHTML = '<span class="value">' + (data.stNum || '0') + '</span>';
                    sqNumDiv.innerHTML = '<span class="value">' + (data.sqNum || '0') + '</span>';
                    document.getElementById('comm').innerHTML = '<span class="value">' + (data.commStatus || 'OK') + '</span>';
                }
            } catch (e) {
                document.getElementById('status').textContent = 'CONNECTION ERROR';
            }
        }
        
        setInterval(updateStatus, 2000);
        updateStatus();
    </script>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🔒 INDRAPRASTHA KINGDOM</h1>
            <div class="subtitle">Southern District Grid - Circuit Breaker v2</div>
        </div>
        
        <div class="mission">
            <div class="mission-title">⚠️ ADVANCED MISSION</div>
            The Southern District powers the kingdom's command & control centers coordinating hostile 
            operations. Your mission: Exploit the IEC 61850 GOOSE protocol to OPEN the circuit breaker 
            and disable the command infrastructure.
        </div>
        
        <div id="status" class="status">LOADING...</div>
        
        <div class="kingdom-status">
            <h3>Kingdom Infrastructure Status</h3>
            <div id="kingdom-msg" style="color: #ffcc99;">Checking...</div>
        </div>
        
        <div id="flag" class="flag hidden"></div>
        
        <div class="info">
            <div class="info-title">📊 IEC 61850 GOOSE Protocol Status</div>
            <div><span class="label">Breaker Position:</span> <span id="position">-</span></div>
            <div><span class="label">Operation Count:</span> <span id="opcount">-</span></div>
            <div><span class="label">State Number (stNum):</span> <span id="stnum">-</span></div>
            <div><span class="label">Sequence Number (sqNum):</span> <span id="sqnum">-</span></div>
            <div><span class="label">Communication Status:</span> <span id="comm">-</span></div>
            <div style="margin-top: 10px;">
                <span class="protocol-badge">IEC 61850-8-1</span>
                <span class="protocol-badge">GOOSE Multicast</span>
                <span class="protocol-badge">192.168.100.0/24</span>
            </div>
        </div>
    </div>
</body>
</html>"""
            self.send_response(200)
            self.send_header('Content-type', 'text/html; charset=utf-8')
            self.send_header('X-Content-Type-Options', 'nosniff')
            self.send_header('X-Frame-Options', 'DENY')
            self.end_headers()
            self.wfile.write(html.encode())
    
    def log_message(self, format, *args):
        pass

with socketserver.TCPServer(("0.0.0.0", PORT), BreakerStatusHandler) as httpd:
    print(f"Breaker IED v2 Status Server running on port {PORT}")
    httpd.serve_forever()