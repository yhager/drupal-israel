function yui_editor_toolbar () {
  this.toolbar = {
    titlebar: '',
    collapse: true,
    draggable: false,
    buttonType: 'advanced',
    buttons: [
        { group: 'textstyle', label: 'Font Style',
            buttons: [
                { type: 'push', label: 'Bold', value: 'bold' },
                { type: 'push', label: 'Italic', value: 'italic' },
                { type: 'separator' },
                { type: 'push', label: 'Subscript', value: 'subscript', disabled: true },
                { type: 'push', label: 'Superscript', value: 'superscript', disabled: true },
                { type: 'separator' },
                { type: 'push', label: 'Remove Formatting', value: 'removeformat', disabled: true },
                { type: 'push', label: 'Show/Hide Hidden Elements', value: 'hiddenelements' }
            ]
        },
        { type: 'separator' },
        { group: 'parastyle', label: 'Paragraph Style',
            buttons: [
            { type: 'select', label: 'Normal', value: 'heading', disabled: true,
                menu: [
                    { text: 'Normal', value: 'none', checked: true },
                    { text: 'Header 1', value: 'h1' },
                    { text: 'Header 2', value: 'h2' },
                    { text: 'Header 3', value: 'h3' },
                    { text: 'Header 4', value: 'h4' },
                    { text: 'Header 5', value: 'h5' },
                    { text: 'Header 6', value: 'h6' }
                ]
            }
            ]
        },
        { type: 'separator' },
        { group: 'indentlist', label: 'Indenting and Lists',
            buttons: [
                { type: 'push', label: 'Indent', value: 'indent', disabled: true },
                { type: 'push', label: 'Outdent', value: 'outdent', disabled: true },
                { type: 'push', label: 'Create an Unordered List', value: 'insertunorderedlist' },
                { type: 'push', label: 'Create an Ordered List', value: 'insertorderedlist' }
            ]
        },
        { type: 'separator' },
        { group: 'insertitem', label: 'Insert Item',
            buttons: [
                { type: 'push', label: 'HTML Link CTRL + SHIFT + L', value: 'createlink', disabled: true },
                { type: 'push', label: 'Insert Image', value: 'insertimage' }
            ]
        },
        { type: 'separator' },
        { group: 'plugins', label: 'Plugins', buttons: [] }
    ]
  };
}
