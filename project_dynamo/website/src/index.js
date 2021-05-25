import "./styles/styles.css";
import { jsPDF } from "jspdf";

//#region main
document.querySelector("#led-on-button").addEventListener("click", () => {
    console.log("Turning LED ON");
    fetch("/led/on");
});
document.querySelector("#led-off-button").addEventListener("click", () => {
    console.log("Turning LED OFF");
    fetch("/led/off");
});

document.querySelector("#pdf-button").addEventListener("click", () => {
    const doc = new jsPDF();

    doc.text("Hello world!", 10, 10);
    doc.save("urkunde.pdf");
});
//#endregion main