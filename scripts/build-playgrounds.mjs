import { spawn } from "node:child_process";
import { copyFile, mkdir, readFile } from "node:fs/promises";
import { dirname, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const repositoryRoot = fileURLToPath(new URL("../", import.meta.url));
const manifestPath = resolve(repositoryRoot, "playgrounds.json");
const destinationDirectory = resolve(
  repositoryRoot,
  "public/assets/playground"
);

const manifest = JSON.parse(await readFile(manifestPath, "utf8"));
const playgrounds = manifest.playgrounds;

if (!Array.isArray(playgrounds) || playgrounds.length === 0) {
  throw new Error("playgrounds.json must define at least one playground.");
}

const requestedIds = process.argv.slice(2).filter((argument) => argument !== "--");
const knownIds = new Set(playgrounds.map((playground) => playground.id));
const unknownIds = requestedIds.filter((id) => !knownIds.has(id));

if (unknownIds.length > 0) {
  throw new Error(
    `Unknown playground${unknownIds.length === 1 ? "" : "s"}: ${unknownIds.join(
      ", "
    )}`
  );
}

const selectedPlaygrounds =
  requestedIds.length === 0
    ? playgrounds
    : playgrounds.filter((playground) => requestedIds.includes(playground.id));

await mkdir(destinationDirectory, { recursive: true });

for (const playground of selectedPlaygrounds) {
  validatePlayground(playground);
  console.log(`\nBuilding ${playground.id}`);

  await run("make", [
    "-C",
    playground.sourceDirectory,
    "image",
    "inspect",
  ]);

  const sourceImage = resolve(
    repositoryRoot,
    playground.sourceDirectory,
    playground.image
  );
  const destinationImage = resolve(
    destinationDirectory,
    `${playground.id}.img`
  );

  await mkdir(dirname(destinationImage), { recursive: true });
  await copyFile(sourceImage, destinationImage);
  console.log(`Copied ${playground.id}.img into the documentation site.`);
}

function validatePlayground(playground) {
  for (const field of ["id", "sourceDirectory", "image"]) {
    if (
      typeof playground[field] !== "string" ||
      playground[field].length === 0
    ) {
      throw new Error(`Every playground requires a non-empty ${field}.`);
    }
  }
}

function run(command, arguments_) {
  return new Promise((resolvePromise, rejectPromise) => {
    const child = spawn(command, arguments_, {
      cwd: repositoryRoot,
      stdio: "inherit",
    });

    child.on("error", rejectPromise);
    child.on("exit", (code, signal) => {
      if (code === 0) {
        resolvePromise();
        return;
      }

      const reason = signal ? `signal ${signal}` : `exit code ${code}`;
      rejectPromise(new Error(`${command} failed with ${reason}.`));
    });
  });
}
