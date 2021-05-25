//Styles
import "./styles/styles.scss";

//JsPDF library
import { jsPDF } from "jspdf";

//#region Buttons
document.querySelector("#led-on-button").addEventListener("click", () => {
    console.log("Turning LED ON");
    fetch("/led/on");
});
document.querySelector("#led-off-button").addEventListener("click", () => {
    console.log("Turning LED OFF");
    fetch("/led/off");
});

document.querySelector("#pdf-button").addEventListener("click", () => {
    const doc: jsPDF = new jsPDF();

    doc.text("Hello world!", 10, 10);
    doc.save("urkunde.pdf");
});
//#endregion Buttons
