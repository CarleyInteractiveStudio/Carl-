import json
import tkinter as tk
from tkinter import filedialog, ttk
import time

class CCPViewer:
    def __init__(self, root):
        self.root = root
        self.root.title("Carley Carl Pensamientos (CCP) Viewer")
        self.root.geometry("900x700")
        self.root.configure(bg="#1e1e1e")

        self.data = None
        self.current_frame = 0
        self.playing = False

        self.setup_ui()

    def setup_ui(self):
        # Header
        header = tk.Label(self.root, text="CCP THOUGHT VIEWER v1.0", font=("Courier", 20, "bold"), fg="#00ff00", bg="#1e1e1e")
        header.pack(pady=10)

        # Main Layout
        main_frame = tk.Frame(self.root, bg="#1e1e1e")
        main_frame.pack(fill=tk.BOTH, expand=True, padx=20)

        # Left: Brain Visualization (Simplified)
        self.canvas = tk.Canvas(main_frame, width=400, height=400, bg="#000000", highlightthickness=1, highlightbackground="#00ff00")
        self.canvas.grid(row=0, column=0, padx=10, pady=10)

        # Right: Stats & Monologue
        side_frame = tk.Frame(main_frame, bg="#1e1e1e")
        side_frame.grid(row=0, column=1, sticky="nsew", padx=10)

        self.monologue_text = tk.Text(side_frame, width=40, height=10, font=("Consolas", 12), bg="#121212", fg="#ffffff", borderwidth=0)
        self.monologue_text.pack(pady=5)

        # Stats Bars
        self.bars = {}
        for stat in ["dopamine", "noradrenaline", "serotonin", "fear", "pain", "hunger", "curiosity"]:
            frame = tk.Frame(side_frame, bg="#1e1e1e")
            frame.pack(fill=tk.X, pady=2)
            tk.Label(frame, text=stat.capitalize(), font=("Consolas", 10), fg="#00ff00", bg="#1e1e1e", width=12, anchor="w").pack(side=tk.LEFT)
            bar = ttk.Progressbar(frame, length=200, mode='determinate')
            bar.pack(side=tk.LEFT, padx=5)
            self.bars[stat] = bar

        # Controls
        ctrl_frame = tk.Frame(self.root, bg="#1e1e1e")
        ctrl_frame.pack(fill=tk.X, side=tk.BOTTOM, pady=20)

        self.btn_load = tk.Button(ctrl_frame, text="OPEN .CCP", command=self.load_file, bg="#00ff00", fg="#000000", font=("Consolas", 12, "bold"))
        self.btn_load.pack(side=tk.LEFT, padx=20)

        self.btn_play = tk.Button(ctrl_frame, text="PLAY", command=self.toggle_play, bg="#0055ff", fg="#ffffff", font=("Consolas", 12, "bold"))
        self.btn_play.pack(side=tk.LEFT, padx=5)

        self.time_label = tk.Label(ctrl_frame, text="TIME: 0.00ms", font=("Consolas", 12), fg="#00ff00", bg="#1e1e1e")
        self.time_label.pack(side=tk.LEFT, padx=20)

        self.slider = ttk.Scale(self.root, from_=0, to=100, orient=tk.HORIZONTAL, command=self.on_slider)
        self.slider.pack(fill=tk.X, padx=40, pady=10)

    def load_file(self):
        path = filedialog.askopenfilename(filetypes=[("CCP Thoughts", "*.ccp"), ("JSON", "*.json")])
        if path:
            with open(path, 'r') as f:
                self.data = json.load(f)
            self.current_frame = 0
            self.slider.config(to=len(self.data['frames']) - 1)
            self.update_frame()

    def toggle_play(self):
        self.playing = not self.playing
        self.btn_play.config(text="PAUSE" if self.playing else "PLAY")
        if self.playing:
            self.play_loop()

    def play_loop(self):
        if self.playing and self.data:
            if self.current_frame < len(self.data['frames']) - 1:
                self.current_frame += 1
                self.slider.set(self.current_frame)
                self.update_frame()
                self.root.after(100, self.play_loop)
            else:
                self.playing = False
                self.btn_play.config(text="PLAY")

    def on_slider(self, val):
        if self.data:
            self.current_frame = int(float(val))
            self.update_frame()

    def update_frame(self):
        if not self.data: return
        frame = self.data['frames'][self.current_frame]

        # Update Time
        self.time_label.config(text=f"TIME: {frame['timestamp']:.2f}ms")

        # Update Monologue
        self.monologue_text.delete(1.0, tk.END)
        self.monologue_text.insert(tk.END, frame.get('inner_monologue', '... SILENCIO ...'))

        # Update Bars
        for stat, val in frame['levels'].items():
            if stat in self.bars:
                self.bars[stat]['value'] = val * 100

        # Update Canvas (Brain Visualization)
        self.canvas.delete("all")
        # Draw some circles for brain regions
        regions = {
            "PFC": (200, 50, "#00ffff"),
            "Amygdala": (150, 200, "#ff0000"),
            "Hippocampus": (250, 200, "#ffff00"),
            "Motor": (200, 100, "#ff00ff"),
            "Insula": (200, 180, "#00ff00"),
            "Wernicke": (120, 250, "#0000ff"),
            "Broca": (280, 250, "#ffaa00")
        }

        for name, (x, y, color) in regions.items():
            # Glow if active (placeholder logic for region activity)
            active = len(frame['active_neurons']) > 0 and (hash(name) % 10 < 3)
            r = 30 if active else 20
            c = color if active else "#333333"
            self.canvas.create_oval(x-r, y-r, x+r, y+r, fill=c, outline="#555555")
            self.canvas.create_text(x, y+r+10, text=name, fill="#ffffff", font=("Consolas", 8))

        # Show active neuron spikes as little dots
        for i, nid in enumerate(frame['active_neurons']):
            nx = 50 + (nid % 20) * 15
            ny = 300 + (nid // 20) * 15
            if ny < 400:
                self.canvas.create_oval(nx-2, ny-2, nx+2, ny+2, fill="#00ff00")

if __name__ == "__main__":
    root = tk.Tk()
    app = CCPViewer(root)
    root.mainloop()
