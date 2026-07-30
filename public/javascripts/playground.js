(function () {
  "use strict";

  var loaderUrl = document.currentScript && document.currentScript.src;
  if (!loaderUrl || !("customElements" in window)) {
    return;
  }

  var runtimeBase = new URL("../assets/vendor/v86/", loaderUrl);
  var imageBase = new URL("../assets/playground/", loaderUrl);
  var v86Module;

  function assetUrl(base, path) {
    return new URL(path, base).href;
  }

  function loadV86() {
    if (!v86Module) {
      v86Module = import(assetUrl(runtimeBase, "libv86.mjs"));
    }
    return v86Module;
  }

  class AOSV86Playground extends HTMLElement {
    connectedCallback() {
      if (this._initialized) {
        return;
      }

      this._initialized = true;
      this._emulator = undefined;
      this._bootCheck = 0;
      this._pausedByUser = false;

      try {
        this._config = this.readConfig();
        this.render();
        this.bindControls();
      } catch (error) {
        this.renderConfigurationError(error);
      }
    }

    disconnectedCallback() {
      this.destroy();
    }

    readConfig() {
      var lesson = this.getAttribute("lesson");
      if (!lesson) {
        throw new Error(
          "The <aos-v86-playground> element requires a lesson attribute."
        );
      }

      var memoryMib = Number(this.getAttribute("memory-mib") || 16);
      if (!Number.isFinite(memoryMib) || memoryMib <= 0) {
        throw new Error("memory-mib must be a positive number.");
      }

      return {
        lesson: lesson,
        image: this.getAttribute("image") || lesson + ".img",
        title: this.getAttribute("title") || lesson,
        expectedOutput: this.getAttribute("expected-output") || "",
        description: this.getAttribute("description") || "",
        memorySize: memoryMib * 1024 * 1024,
      };
    }

    render() {
      this.classList.add("aos-playground");
      this.innerHTML = [
        '<div class="aos-playground__header">',
        "  <div>",
        '    <span class="aos-playground__eyebrow">AOS browser lab</span>',
        '    <h4 class="aos-playground__title" data-v86-title></h4>',
        '    <p class="aos-playground__description" data-v86-description></p>',
        "  </div>",
        '  <div class="aos-playground__status" data-v86-status data-state="idle" role="status" aria-live="polite">',
        '    <span class="aos-playground__status-dot" aria-hidden="true"></span>',
        "    <span data-v86-status-text>Ready to start</span>",
        "  </div>",
        "</div>",
        '<div class="aos-playground__controls" aria-label="Emulator controls">',
        '  <button class="aos-playground__button aos-playground__button--primary" type="button" data-v86-action="start">Start emulator</button>',
        '  <button class="aos-playground__button" type="button" data-v86-action="pause" disabled>Pause</button>',
        '  <button class="aos-playground__button" type="button" data-v86-action="reset" disabled>Reset</button>',
        "</div>",
        '<div class="aos-playground__display">',
        '  <div class="aos-playground__placeholder" data-v86-placeholder>',
        "    <strong>PC powered off</strong>",
        "    <span>Press Start emulator to boot the lesson image.</span>",
        "  </div>",
        '  <div class="aos-playground__screen" data-v86-screen>',
        '    <div class="aos-playground__text-screen"></div>',
        '    <canvas class="aos-playground__canvas"></canvas>',
        "  </div>",
        "</div>",
        '<div class="aos-playground__footer">',
        "  <span>The first start downloads the emulator runtime, BIOS, and floppy image.</span>",
        '  <details class="aos-playground__error" data-v86-error hidden>',
        "    <summary>Technical details</summary>",
        "    <pre data-v86-error-text></pre>",
        "  </details>",
        "</div>",
      ].join("\n");

      this.querySelector("[data-v86-title]").textContent = this._config.title;

      var description = this.querySelector("[data-v86-description]");
      if (this._config.description) {
        description.textContent = this._config.description;
      } else {
        description.append("SeaBIOS will boot ");
        var imageName = document.createElement("code");
        imageName.textContent = this._config.image;
        description.append(imageName, " as a 1.44 MiB floppy disk.");
      }

      this.querySelector("[data-v86-screen]").setAttribute(
        "aria-label",
        "Emulated PC display for " + this._config.title
      );

      this._screen = this.querySelector("[data-v86-screen]");
      this._placeholder = this.querySelector("[data-v86-placeholder]");
      this._status = this.querySelector("[data-v86-status]");
      this._statusText = this.querySelector("[data-v86-status-text]");
      this._startButton = this.querySelector('[data-v86-action="start"]');
      this._pauseButton = this.querySelector('[data-v86-action="pause"]');
      this._resetButton = this.querySelector('[data-v86-action="reset"]');
      this._errorDetails = this.querySelector("[data-v86-error]");
      this._errorText = this.querySelector("[data-v86-error-text]");
    }

    renderConfigurationError(error) {
      this.classList.add("aos-playground", "aos-playground--error");
      this.innerHTML = [
        '<div class="aos-playground__header">',
        "  <div>",
        '    <span class="aos-playground__eyebrow">AOS browser lab</span>',
        '    <h4 class="aos-playground__title">Playground configuration error</h4>',
        "  </div>",
        "</div>",
        '<div class="aos-playground__footer">',
        '  <pre class="aos-playground__configuration-error"></pre>',
        "</div>",
      ].join("\n");
      this.querySelector(".aos-playground__configuration-error").textContent =
        error instanceof Error ? error.message : String(error);
    }

    bindControls() {
      this._startButton.addEventListener("click", () => this.start());
      this._pauseButton.addEventListener("click", () => this.togglePause());
      this._resetButton.addEventListener("click", () => this.reset());
    }

    setStatus(state, message) {
      this._status.dataset.state = state;
      this._statusText.textContent = message;
    }

    showError(error) {
      var message = error instanceof Error ? error.message : String(error);
      this.setStatus("error", "Could not start");
      this._errorText.textContent = message;
      this._errorDetails.hidden = false;
      this._startButton.disabled = false;
      this._startButton.textContent = "Try again";
      this._pauseButton.disabled = true;
      this._resetButton.disabled = true;
      this._placeholder.hidden = false;
      this._placeholder.querySelector("strong").textContent =
        "Emulator unavailable";
      this._placeholder.querySelector("span").textContent =
        "Check your connection, then try again.";
    }

    async verifyBoot() {
      if (!this._config.expectedOutput) {
        this.setStatus("running", "Running");
        return;
      }

      var currentCheck = ++this._bootCheck;
      this.setStatus("booting", "Booting floppy…");

      try {
        var found = await this._emulator.wait_until_vga_screen_contains(
          this._config.expectedOutput,
          { timeout_msec: 15000 }
        );

        if (currentCheck !== this._bootCheck) {
          return;
        }

        this.setStatus(
          found ? "verified" : "running",
          found ? "Boot verified" : "Running"
        );
      } catch (error) {
        if (currentCheck === this._bootCheck) {
          this.setStatus("running", "Running");
        }
      }
    }

    async start() {
      this._startButton.disabled = true;
      this._startButton.textContent = "Loading…";
      this._errorDetails.hidden = true;
      this._placeholder.hidden = false;
      this._placeholder.querySelector("strong").textContent =
        "Starting virtual PC";
      this._placeholder.querySelector("span").textContent =
        "Loading the emulator and lesson image…";
      this.setStatus("loading", "Loading assets…");

      try {
        if (this._emulator) {
          await this._emulator.destroy();
          this._emulator = undefined;
        }

        var module = await loadV86();

        this._emulator = new module.V86({
          wasm_path: assetUrl(runtimeBase, "v86.wasm"),
          memory_size: this._config.memorySize,
          vga_memory_size: 2 * 1024 * 1024,
          screen_container: this._screen,
          bios: {
            url: assetUrl(runtimeBase, "seabios.bin"),
          },
          vga_bios: {
            url: assetUrl(runtimeBase, "vgabios.bin"),
          },
          fda: {
            url: assetUrl(imageBase, this._config.image),
          },
          boot_order: 0x231,
          disable_mouse: true,
          disable_speaker: true,
          autostart: true,
        });

        this._emulator.add_listener("emulator-ready", () => {
          this._placeholder.hidden = true;
          this._startButton.textContent = "Running";
          this._pauseButton.disabled = false;
          this._resetButton.disabled = false;
          this.verifyBoot();
        });

        this._emulator.add_listener("emulator-started", () => {
          this._pausedByUser = false;
          this._pauseButton.textContent = "Pause";
        });

        this._emulator.add_listener("emulator-stopped", () => {
          if (this._pausedByUser) {
            this.setStatus("paused", "Paused");
            this._pauseButton.textContent = "Resume";
          }
        });

        this._emulator.add_listener("download-error", (event) => {
          var failedFile =
            event && event.file_name
              ? "Unable to load " + event.file_name
              : "Asset download failed";
          this.showError(new Error(failedFile));
        });
      } catch (error) {
        this.showError(error);
      }
    }

    async togglePause() {
      if (!this._emulator) {
        return;
      }

      this._pauseButton.disabled = true;

      try {
        if (this._emulator.is_running()) {
          this._pausedByUser = true;
          ++this._bootCheck;
          await this._emulator.stop();
        } else {
          this._pausedByUser = false;
          await this._emulator.run();
          this.setStatus("running", "Running");
          this._pauseButton.textContent = "Pause";
        }
      } finally {
        this._pauseButton.disabled = false;
      }
    }

    reset() {
      if (!this._emulator) {
        return;
      }

      ++this._bootCheck;
      this._emulator.restart();
      this._pauseButton.textContent = "Pause";
      this.verifyBoot();
    }

    async destroy() {
      ++this._bootCheck;
      if (this._emulator) {
        await this._emulator.destroy();
        this._emulator = undefined;
      }
    }
  }

  if (!customElements.get("aos-v86-playground")) {
    customElements.define("aos-v86-playground", AOSV86Playground);
  }

  function upgradeMarkdownPlaceholders() {
    document.querySelectorAll("[data-aos-v86-playground]").forEach(
      function (placeholder) {
        var playground = document.createElement("aos-v86-playground");

        Array.from(placeholder.attributes).forEach(function (attribute) {
          if (attribute.name !== "data-aos-v86-playground") {
            playground.setAttribute(attribute.name, attribute.value);
          }
        });

        placeholder.replaceWith(playground);
      }
    );
  }

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", upgradeMarkdownPlaceholders, {
      once: true,
    });
  } else {
    upgradeMarkdownPlaceholders();
  }

  window.addEventListener("pagehide", function () {
    document.querySelectorAll("aos-v86-playground").forEach(function (element) {
      element.destroy();
    });
  });
})();
