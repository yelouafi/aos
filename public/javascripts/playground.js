(function () {
  "use strict";

  var loaderUrl = document.currentScript && document.currentScript.src;
  if (!loaderUrl) {
    return;
  }

  var runtimeBase = new URL("../assets/vendor/v86/", loaderUrl);
  var imageBase = new URL("../assets/playground/", loaderUrl);
  var v86Module;
  var activePlaygrounds = new Set();

  function assetUrl(base, path) {
    return new URL(path, base).href;
  }

  function loadV86() {
    if (!v86Module) {
      v86Module = import(assetUrl(runtimeBase, "libv86.mjs"));
    }
    return v86Module;
  }

  function initializePlayground(root) {
    if (root.dataset.v86Initialized === "true") {
      return;
    }

    root.dataset.v86Initialized = "true";
    activePlaygrounds.add(root);

    var lesson = root.dataset.v86Playground;
    var screen = root.querySelector("[data-v86-screen]");
    var placeholder = root.querySelector("[data-v86-placeholder]");
    var status = root.querySelector("[data-v86-status]");
    var statusText = root.querySelector("[data-v86-status-text]");
    var startButton = root.querySelector('[data-v86-action="start"]');
    var pauseButton = root.querySelector('[data-v86-action="pause"]');
    var resetButton = root.querySelector('[data-v86-action="reset"]');
    var errorDetails = root.querySelector("[data-v86-error]");
    var errorText = root.querySelector("[data-v86-error-text]");

    var emulator;
    var bootCheck = 0;
    var pausedByUser = false;

    function setStatus(state, message) {
      status.dataset.state = state;
      statusText.textContent = message;
    }

    function showError(error) {
      var message = error instanceof Error ? error.message : String(error);
      setStatus("error", "Could not start");
      errorText.textContent = message;
      errorDetails.hidden = false;
      startButton.disabled = false;
      startButton.textContent = "Try again";
      pauseButton.disabled = true;
      resetButton.disabled = true;
      placeholder.hidden = false;
      placeholder.querySelector("strong").textContent = "Emulator unavailable";
      placeholder.querySelector("span").textContent =
        "Check your connection, then try again.";
    }

    async function verifyBoot() {
      var currentCheck = ++bootCheck;
      setStatus("booting", "Booting floppy…");

      try {
        var found = await emulator.wait_until_vga_screen_contains(
          "Peace be upon you!",
          { timeout_msec: 15000 }
        );

        if (currentCheck !== bootCheck) {
          return;
        }

        setStatus(
          found ? "verified" : "running",
          found ? "Boot verified" : "Running"
        );
      } catch (error) {
        if (currentCheck === bootCheck) {
          setStatus("running", "Running");
        }
      }
    }

    async function start() {
      startButton.disabled = true;
      startButton.textContent = "Loading…";
      errorDetails.hidden = true;
      placeholder.hidden = false;
      placeholder.querySelector("strong").textContent = "Starting virtual PC";
      placeholder.querySelector("span").textContent =
        "Loading the emulator and lesson image…";
      setStatus("loading", "Loading assets…");

      try {
        if (emulator) {
          await emulator.destroy();
          emulator = undefined;
        }

        var module = await loadV86();

        emulator = new module.V86({
          wasm_path: assetUrl(runtimeBase, "v86.wasm"),
          memory_size: 16 * 1024 * 1024,
          vga_memory_size: 2 * 1024 * 1024,
          screen_container: screen,
          bios: {
            url: assetUrl(runtimeBase, "seabios.bin"),
          },
          vga_bios: {
            url: assetUrl(runtimeBase, "vgabios.bin"),
          },
          fda: {
            url: assetUrl(imageBase, lesson + ".img"),
          },
          boot_order: 0x231,
          disable_mouse: true,
          disable_speaker: true,
          autostart: true,
        });

        emulator.add_listener("emulator-ready", function () {
          placeholder.hidden = true;
          startButton.textContent = "Running";
          pauseButton.disabled = false;
          resetButton.disabled = false;
          verifyBoot();
        });

        emulator.add_listener("emulator-started", function () {
          pausedByUser = false;
          pauseButton.textContent = "Pause";
        });

        emulator.add_listener("emulator-stopped", function () {
          if (pausedByUser) {
            setStatus("paused", "Paused");
            pauseButton.textContent = "Resume";
          }
        });

        emulator.add_listener("download-error", function (event) {
          var failedFile =
            event && event.file_name ? "Unable to load " + event.file_name : "Asset download failed";
          showError(new Error(failedFile));
        });
      } catch (error) {
        showError(error);
      }
    }

    async function togglePause() {
      if (!emulator) {
        return;
      }

      pauseButton.disabled = true;

      try {
        if (emulator.is_running()) {
          pausedByUser = true;
          ++bootCheck;
          await emulator.stop();
        } else {
          pausedByUser = false;
          await emulator.run();
          setStatus("running", "Running");
          pauseButton.textContent = "Pause";
        }
      } finally {
        pauseButton.disabled = false;
      }
    }

    function reset() {
      if (!emulator) {
        return;
      }

      ++bootCheck;
      emulator.restart();
      pauseButton.textContent = "Pause";
      verifyBoot();
    }

    async function destroy() {
      ++bootCheck;
      if (emulator) {
        await emulator.destroy();
        emulator = undefined;
      }
      activePlaygrounds.delete(root);
    }

    startButton.addEventListener("click", start);
    pauseButton.addEventListener("click", togglePause);
    resetButton.addEventListener("click", reset);
    root._destroyV86Playground = destroy;
  }

  function initializeAll() {
    activePlaygrounds.forEach(function (root) {
      if (!document.contains(root) && root._destroyV86Playground) {
        root._destroyV86Playground();
      }
    });

    document
      .querySelectorAll("[data-v86-playground]")
      .forEach(initializePlayground);
  }

  if (typeof document$ !== "undefined") {
    document$.subscribe(initializeAll);
  } else if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", initializeAll);
  } else {
    initializeAll();
  }

  window.addEventListener("pagehide", function () {
    activePlaygrounds.forEach(function (root) {
      if (root._destroyV86Playground) {
        root._destroyV86Playground();
      }
    });
  });
})();
