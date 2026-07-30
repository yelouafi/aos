import { readFileSync } from "node:fs";
import { expect, test } from "@playwright/test";

const manifest = JSON.parse(
  readFileSync(new URL("../../playgrounds.json", import.meta.url), "utf8")
);

for (const playground of manifest.playgrounds) {
  test(`${playground.id} boots to its expected output`, async ({ page }) => {
    await page.goto(playground.page);

    const emulator = page.locator("aos-v86-playground");
    await expect(emulator).toHaveCount(1);
    await expect(emulator.getByRole("status")).toHaveText("Ready to start");

    await emulator.getByRole("button", { name: "Start emulator" }).click();

    await expect(emulator.getByRole("status")).toHaveText("Boot verified");
    await expect(emulator.locator("[data-v86-screen]")).toContainText(
      playground.expectedOutput
    );
    await expect(emulator.locator("[data-v86-error]")).toBeHidden();
  });
}
