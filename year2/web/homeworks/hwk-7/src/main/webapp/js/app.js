window.notify = function (message) {
    $.notify(message, {
        position: "right bottom",
        className: "success"
    });
}

window.notifyError = function (message) {
    $.notify(message, {
        position: "right bottom",
        className: "error"
    });
}

$.fn.exists = function () {
    return this.length !== 0;
}

// Simple cache since jquery doesn't use one by default
const jqueryCache = new Map();
function $cached(...any) {
    return jqueryCache.get(any) || $(...any);
}

function toggleText($element, values) {
    if ($element.text() === values[0]) {
        $element.text(values[1]);
    } else {
        $element.text(values[0]);
    }
}

const vals = (() => {
    // formValue extracts the value of some user-input-kind tag inside the form element
    function formValue(form, tagName, attributeName, attributeValue) {
        return $cached(form).find(`${tagName}[${attributeName}='${attributeValue}']`).val();
    }

    function formValueExtractor(tagName, attributeName) {
        return (form, attributeValue) => formValue(form, tagName, attributeName, attributeValue);
    }

    // inputValue returns a function which extracts an input field from the form
    const inputValue = formValueExtractor("input", "name");
    // textareaValue returns a function which extracts a textarea field from the form
    const textareaValue = formValueExtractor("textarea", "name");

    // attributeValue extracts the value of an element attribute specified by attributeName
    function attributeValue(element, attributeName) {
        return $cached(element).attr(attributeName);
    }

    // attributeValueExtractor returns a function which extracts the specified attribute
    function attributeValueExtractor (attributeName) {
        return (element) => attributeValue(element, attributeName);
    }

    function dataAttributeValue (element, attributeName) {
        return attributeValue(element, `data-${attributeName}`);
    }

    return { formValue, formValueExtractor, inputValue, textareaValue,
             attributeValue, attributeValueExtractor, dataAttributeValue };
})();

// requests module
const requests = (() => {
    function defaultHandler(customHandler, response) {
        if (response.error) {
            const $errorEl = $cached(this).find(".error");
            if ($errorEl.exists()) {
                $errorEl.text(response.error);
            } else {
                notifyError(response.error);
            }
        } else if (response.redirect) {
            location.href = response.redirect;
        } else if (customHandler) {
            customHandler.bind($cached(this))(response);
        }
    }

    function resolve(lazyData, element) {
        const data = {};
        for (const key in lazyData) {
            if (lazyData[key] instanceof Function) {
                data[key] = lazyData[key](element, key);
            } else {
                data[key] = lazyData[key];
            }
        }
        return data;
    }

    // Request class wrapping jquery ajax parameters and incapsulating data extraction logic
    class Request {
        ajaxConfig;
        data;
        handler;

        constructor(prettyAction, data, handler) {
            const spaceIndex = prettyAction.indexOf(' ');
            if (spaceIndex === -1) {
                throw Error("Invalid action specified for requests.Run: " + prettyAction);
            }
            const method = prettyAction.slice(0, spaceIndex);
            let action = prettyAction.slice(spaceIndex + 1);

            const slashIndex = action.lastIndexOf('/');
            let url = ""
            if (slashIndex !== -1) {
                url = action.slice(0, slashIndex);
                action = action.slice(slashIndex + 1);
            }

            this.ajaxConfig = {
                method,
                url,
                dataType: "json",
            };
            this.data = {
                ...data,
                action
            };
            this.handler = handler;
        }

        execute(element) {
            const ajaxConfig = {...this.ajaxConfig};
            ajaxConfig.data = resolve(this.data, element);
            ajaxConfig.success = defaultHandler.bind(element, this.handler);
            $.ajax(ajaxConfig);
        }
    }

    // Returns a valid EventHandler which will run the request constructed using parameters on the target element of
    function Run(request) {
        return (event) => {
            event.preventDefault();
            event.stopPropagation();
            request.execute(event.currentTarget);
        }
    }

    return { Request, Run }
})()

// template modules
const template = (() => {
    // extractNamespacedKey gets value in data as specified by namespaced key (a.b.c)
    function extractNamespacedKey(data, namespacedKey) {
        let value = data;
        namespacedKey.split(".").forEach(key => {
            value = value[key];
        })
        return value;
    }

    class Template {
        template;
        tmplFormat;
        output;

        constructor(templateSelector, format, outputSelector) {
            this.template = $($(templateSelector).prop("content"));
            this.tmplFormat = format;
            this.output = $(outputSelector);
        }

        // Format a specific element's attributes
        formatAttributes(data, $tmpl, attributeFormat) {
            for (const [key, value] of Object.entries(attributeFormat)) {
                $tmpl.attr(key, extractNamespacedKey(data, value));
            }
        }

        // Format an element recursively
        formatElement(data, $tmpl, format) {
            for (const [key, value] of Object.entries(format)) {
                if (value instanceof Function) {
                    $tmpl.find(key).text(value(data));
                } else if (value instanceof Object) {
                    if (key === "attributes") {
                        this.formatAttributes(data, $tmpl, value);
                    } else {
                        this.formatElement(data, $tmpl.find(key), value);
                    }
                } else {
                    $tmpl.find(key).text(extractNamespacedKey(data, value));
                }
            }
        }

        formatOne(data) {
            const $tmpl = this.template.clone();
            this.formatElement(data, $tmpl, this.tmplFormat);
            this.output.append($tmpl);
        }

        format(data) {
            data.forEach((item) => this.formatOne(item));
        }
    }
    return { Template, extractNamespacedKey }
})();