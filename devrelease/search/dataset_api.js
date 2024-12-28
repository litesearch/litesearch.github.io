// API For Creating DataSets
function dataset_Export(title, url, description){
    document.getElementById('results').innerHTML += `<button class="websites">
            <h2 class="title">
                ${title}
            </h2>
            <h4 class="url">
                ${url}
            </h4>
            <p class="description">
                ${description}
            </p>
        </button><br><br>`;
}

function export_Website(content){
    document.getElementById('results').innerHTML += content;
}

function clear_Exports(){
    document.getElementById('results').innerHTML = '';
}