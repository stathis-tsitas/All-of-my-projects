window.onload = function(){
    let currentNumber = '0';
    let display = this.document.getElementById('current');
    let previousNumber = '';
    let operator = null;
    let justCalculated = false;

    function updateDisplay() {
    let num = parseFloat(currentNumber);
    
    if (!isNaN(num)) {
        let formatted = num.toLocaleString('el-GR', {
            maximumFractionDigits: 8,
            useGrouping: true
        });
        display.textContent = formatted;
    } else {
        display.textContent = currentNumber;
    }
}

    updateDisplay();

    let numberButtons = document.querySelectorAll('.number');

    numberButtons.forEach(function(button){
        button.onclick = function(){
            let number = button.textContent;
             if (justCalculated) {           
                currentNumber = '0';
                justCalculated = false;
        }
            if(currentNumber === '0'){
                currentNumber = number;
            } else {
                currentNumber = currentNumber + number;
            }

            updateDisplay();
        };
    });

    let operatorButtons = document.querySelectorAll('.operator');
    operatorButtons.forEach(function(button){
        button.onclick = function(){
            operator = button.textContent;
            previousNumber = currentNumber;
            currentNumber = '0';
        };
    });

    let equalsButton = document.querySelector('.equals');
    equalsButton.onclick = function() {
    
    if (!operator || previousNumber === '') {
        return;
    }
    
    let result;
    let prev = parseFloat(previousNumber);
    let current = parseFloat(currentNumber);
    
    if (operator === '+') {
        result = prev + current;
    } else if (operator === '-') {
        result = prev - current;
    } else if (operator === '×') {
        result = prev * current;
    } else if (operator === '÷') {
        result = prev / current;
    }
    
    currentNumber = result.toString();
    operator = null;
    previousNumber = '';
    justCalculated = true;
    updateDisplay();
};
let clearButton = document.querySelector('.clear');
clearButton.onclick = function() {
    currentNumber = '0';
    previousNumber = '';
    operator = null;
    updateDisplay();
};
let deleteButton = document.querySelector('.delete');
deleteButton.onclick = function() {
    if (currentNumber.length === 1) {
        currentNumber = '0';
    } else {
        currentNumber = currentNumber.slice(0, -1); // Remove last character
    }
    updateDisplay();
};

let percentButton = document.querySelector('.percent');
percentButton.onclick = function() {
    let current = parseFloat(currentNumber);
    
    if (operator && previousNumber !== '') {
        let prev = parseFloat(previousNumber);
        let percentage = (prev * current) / 100;
        
        if (operator === '+') {
            currentNumber = (prev + percentage).toString();
        } else if (operator === '-') {
            currentNumber = (prev - percentage).toString();
        } else if (operator === '×') {
            currentNumber = percentage.toString();
        } else if (operator === '÷') {
            currentNumber = (prev / (current / 100)).toString();
        }
        
        operator = null;
        previousNumber = '';
    } else {
        currentNumber = (current / 100).toString();
    }
    
    updateDisplay();
};
let plusMinusButton = document.querySelector('.plus-minus');
plusMinusButton.onclick = function() {
    let num = parseFloat(currentNumber);
    
    if (num !== 0) {
        currentNumber = (num * -1).toString();
        updateDisplay();
    }
};
};