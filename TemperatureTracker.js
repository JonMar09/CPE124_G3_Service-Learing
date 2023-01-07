export default class TemperatureTracker {
    constructor(root) {
        this.root = root;
        this.root.insertAdjacentHTML("afterbegin", TemperatureTracker.html());

        this.loadEntries();
        this.updateView();

        this.root.querySelector(".temperature__add").addEventListener("click", () => {
            const date = new Date();
            const year = date.getFullYear();
            const month = (date.getMonth() +1).toString().padStart(2, "0");
            const day = date.getDate().toString().padStart(2, "0");

            console.log(year, month, day)
            this.addEntry({
                date: `${ year }-${ month }-${ day }`,
                temperature: 5,
                duration: 30

            });
            
            
        });
    }


    static html() {
        return `
            <table class="temperature">
                        <thread>
                            <tr>
                                <th>Date</th>
                                <th>Temperature</th>
                                <th>Cycles</th>
                                <th></th>
                            </tr>
                        </thread>
                        <tbody class="temperature__entries"></tbody>
                        <tbody>
                            <tr class="temperature__row temperature__row--add">
                                <td colspan="4">
                                    <span class="temperature__add">Add Entry &plus;</span>
                                </td>
                            </tr>
                        </tbody>
                    </table>
        `;
    }

    static rowHtml() {
        return `
                    <tr class="temperature__row">
                    <td>
                        <input type="date" class="temperature__date">
                    </td>
                    <td>
                        <input type="number" class="tracker__temperature">
                        <span class="temperature__degrees">celcius</span>
                    </td>
                    <td>
                        <input type="number" class="temperature__duration">
                        <span class="temperature__test">minutes</span>
                    </td>
                    <td>
                        <button type="button" class="temperature__button temperature__delete">&times;</button>
                    </td>
                </tr>
        `;
    }

    loadEntries() {
        this.entries = JSON.parse(localStorage.getItem("temperature-tracker-entries"));
    }

    saveEntries() {
        localStorage.setItem("temperature-tracker-entries", JSON.stringify(this.entries));
    }

    updateView() {
        const tableBody = this.root.querySelector(".temperature__entries");
        const addRow = data => {
            const template = document.createElement("template");
            let row = null;
            
            template.innerHTML = TemperatureTracker.rowHtml().trim();
            row = template.content.firstElementChild;

            row.querySelector(".temperature__date").value = data.date;
            row.querySelector(".tracker__temperature").value = data.workout;
            row.querySelector(".temperature__duration").value = data.duration;

            row.querySelector(".temperature__date").addEventListener("change", ({ target }) => {
                data.date = target.value;
                this.saveEntries();
            });

            row.querySelector(".tracker__temperature").addEventListener("change", ({ target }) => {
                data.workout = target.value;
                this.saveEntries();
            });

            row.querySelector(".temperature__duration").addEventListener("change", ({ target }) => {
                data.duration = target.value;
                this.saveEntries();
            });
            
            row.querySelector(".temperature__delete").addEventListener("click", () => {
                this.deleteEntry(data);
            });


            // TODO: add events for input changing
            //TODO: add event to delete the row

            tableBody.appendChild(row);
        };

        tableBody.querySelectorAll(".temperature__row").forEach( row => {
            row.remove();
        });

        this.entries.forEach(data => addRow(data));
    }

    addEntry(data) {
        this.entries.push(data);
        this.saveEntries();
        this.updateView();

    }

    deleteEntry(dataToDelete) {
        this.entries = this.entries.filter(data => data !== dataToDelete);
        this.saveEntries();
        this.updateView();

    }
}

